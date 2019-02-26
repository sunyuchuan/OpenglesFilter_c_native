/*
* Created by sunyc on 2018-8-22.
* time filter ghost.
*/
#include "ijksdl/gles2_xm/internal.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"

typedef struct XM_TFilter_ghost
{
    XM_GLES2_Renderer *renderer;
    GLuint us2_sampler[XM_GLES2_MAX_PLANE];
    GLuint plane_textures[XM_GLES2_MAX_PLANE];
    GLuint av2_texcoord;
    GLfloat texcoords[8];
    GLfloat step;
} XM_TFilter_ghost;

typedef struct XM_Filter_Opaque {
    XM_TFilter_ghost *filter;
} XM_Filter_Opaque;

static void XM_Filter_TexCoords_Ghost_reload(XM_TFilter_ghost *filter)
{
    glVertexAttribPointer(filter->av2_texcoord, 2, GL_FLOAT, GL_FALSE, 0, filter->texcoords);   XM_GLES2_checkError_TRACE("glVertexAttribPointer(av2_texcoord_ghost)");
    glEnableVertexAttribArray(filter->av2_texcoord);                                              XM_GLES2_checkError_TRACE("glEnableVertexAttribArray(av2_texcoord_ghost)");
}

static void XM_Filter_TexCoords_Ghost_reset(XM_TFilter_ghost *filter, GLfloat step, GLfloat cropRight)
{
    if(isnan(cropRight))
        cropRight = 0.0f;
    filter->texcoords[0] = 0.0f + step;
    filter->texcoords[1] = 1.0f - step;
    filter->texcoords[2] = (1.0f - cropRight) * (1.0f - step);
    filter->texcoords[3] = 1.0f - step;
    filter->texcoords[4] = 0.0f + step;
    filter->texcoords[5] = 0.0f + step;
    filter->texcoords[6] = (1.0f - cropRight) * (1.0f - step);
    filter->texcoords[7] = 0.0f + step;
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;
    XM_TFilter_ghost *filter = opaque->filter;

    if(filter->step <= 3*GHOST_TEXTURE_SCALE_RANGE/5)
        filter->step += (3*GHOST_TEXTURE_SCALE_RANGE/5)/GHOST_FRAME_INTERVAL_NUM_MAX;
    else if(filter->step <= GHOST_TEXTURE_SCALE_RANGE)
        filter->step += (2*GHOST_TEXTURE_SCALE_RANGE/5)/GHOST_FRAME_INTERVAL_NUM_MAX;
    else if(filter->step >= GHOST_TEXTURE_SCALE_RANGE)
        filter->step = 0.0f;

    XM_Filter_TexCoords_Ghost_reset(filter, filter->step, filter->renderer->cropRight);
    if(param->off_screen)
    {
        XM_Filter_TexCoords_Rotation(filter->texcoords, ROTATION_180, false, false);
    }
    XM_Filter_TexCoords_Ghost_reload(filter);
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_ghost *filter = opaque->filter;
    XM_Filter_TexCoords_Ghost_reset(filter, 0.0f, filter->renderer->cropRight);
    if(param->off_screen)
    {
        XM_Filter_TexCoords_Rotation(filter->texcoords, ROTATION_180, false, false);
    }
    XM_Filter_TexCoords_Ghost_reload(filter);
}

static bool yuv420p_uploadTexture(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    if (!opaque || !opaque->filter || !overlay)
        return false;

    XM_TFilter_ghost *filter = opaque->filter;
    int	  planes[3]    = { 0, 1, 2};
    const GLsizei widths[3]    = {overlay->veffects_pitches[0], overlay->veffects_pitches[1], overlay->veffects_pitches[2] };
    const GLsizei heights[3]   = {overlay->veffects_h,		  overlay->veffects_h / 2,		overlay->veffects_h / 2 };
    const GLubyte *pixels[3]   = {overlay->veffects_pixels[0],  overlay->veffects_pixels[1],  overlay->veffects_pixels[2] };

    switch (overlay->veffects_format) {
        case SDL_FCC_I420:
            break;
        case SDL_FCC_YV12:
            planes[1] = 2;
            planes[2] = 1;
            break;
        default:
            ALOGE("[yuv420p] unexpected veffects_format %x\n", overlay->veffects_format);
            return false;
    }

    for (int i = 0; i < 3; ++i) {
        int plane = planes[i];

        glBindTexture(GL_TEXTURE_2D, filter->plane_textures[i]);

        glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_LUMINANCE,
                widths[plane],
                heights[plane],
                0,
                GL_LUMINANCE,
                GL_UNSIGNED_BYTE,
                pixels[plane]);
    }

    return true;
}

static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    if (!opaque || !opaque->filter || !opaque->filter->renderer)
        return 0;

    XM_TFilter_ghost *filter = opaque->filter;
    int f_textures_num = 3;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glUseProgram(filter->renderer->program);            XM_GLES2_checkError_TRACE("glUseProgram");

    if (0 == filter->plane_textures[0])
        glGenTextures(f_textures_num, filter->plane_textures);

    for (int i = 0; i < f_textures_num; ++i) {
        glActiveTexture(GL_TEXTURE0 + i + textures_num);
        glBindTexture(GL_TEXTURE_2D, filter->plane_textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glUniform1i(filter->us2_sampler[i], i + textures_num);
    }

    return f_textures_num;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter || !opaque->filter->renderer)
        return;

    XM_TFilter_ghost *filter = opaque->filter;
    filter->us2_sampler[0] = glGetUniformLocation(filter->renderer->program, "us2_SamplerX_ghost"); XM_GLES2_checkError_TRACE("glGetUniformLocation(us2_SamplerX_ghost)");
    filter->us2_sampler[1] = glGetUniformLocation(filter->renderer->program, "us2_SamplerY_ghost"); XM_GLES2_checkError_TRACE("glGetUniformLocation(us2_SamplerY_ghost)");
    filter->us2_sampler[2] = glGetUniformLocation(filter->renderer->program, "us2_SamplerZ_ghost"); XM_GLES2_checkError_TRACE("glGetUniformLocation(us2_SamplerZ_ghost)");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter || !opaque->filter->renderer)
        return;

    XM_TFilter_ghost *filter = opaque->filter;
    filter->av2_texcoord = glGetAttribLocation(filter->renderer->program, "av2_Texcoord_ghost");				  XM_GLES2_checkError_TRACE("glGetAttribLocation(av2_Texcoord_ghost)");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_ghost *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_TimeFilter_getVertexShader_ghost();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_TFilter_yuv420p_getFragmentShader_ghost();
}

static void XM_TFilter_reset(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;
    XM_TFilter_ghost *filter = opaque->filter;

    for (int i = 0; i < XM_GLES2_MAX_PLANE; ++i) {
        if (filter->plane_textures[i]) {
            glDeleteTextures(1, &filter->plane_textures[i]);
            filter->plane_textures[i] = 0;
        }
    }
}

static void XM_TFilter_Ghost_free(XM_TFilter_ghost *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Ghost_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_TFilter_ghost));
    free(filter);
}

static void XM_TFilter_Ghost_freep(XM_TFilter_ghost **filter)
{
    if (!filter || !*filter)
        return;

    XM_TFilter_Ghost_free(*filter);
    *filter = NULL;
}

static void XM_TFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_Ghost_freep(&opaque->filter);
}

XM_Filter *XM_TFilter_Ghost_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_TFilter_ghost *)calloc(1, sizeof(XM_TFilter_ghost));

    filter->func_free = XM_TFilter_free;
    filter->func_reset = XM_TFilter_reset;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_VertexShader_init = vsh_init;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_yuv420p_use = yuv420p_use;
    filter->func_yuv420p_uploadTexture= yuv420p_uploadTexture;
    filter->func_ShaderVariables_reset = shaderVariables_reset;
    filter->func_setShaderVariables = setShaderVariables;

    return filter;
}


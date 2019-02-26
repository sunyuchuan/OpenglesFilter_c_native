/*
* Created by sunyc on 2018-8-22.
* base filter fairytale.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"
#include "ijksdl/xm_bitmap.h"

typedef struct XM_BFilter_fairytale
{
    XM_GLES2_Base_Renderer *renderer;
    XM_Bitmap *xm_bmp;
    GLuint us2_sampler_1;
    GLuint av2_texcoord_1;
    GLuint textures[1];
    GLfloat texcoords[8];
} XM_BFilter_fairytale;

typedef struct XM_Filter_Opaque {
    XM_BFilter_fairytale *filter;
} XM_Filter_Opaque;

static void XM_Fairytale_TexCoords_reset(XM_BFilter_fairytale *filter, float crop, bool crop_w)
{
    if(isnan(crop) || !filter)
        return;

    if(crop_w) {
        filter->texcoords[0] = 0.0f;
        filter->texcoords[1] = 1.0f;
        filter->texcoords[2] = 1.0f - crop;
        filter->texcoords[3] = 1.0f;
        filter->texcoords[4] = 0.0f;
        filter->texcoords[5] = 0.0f;
        filter->texcoords[6] = 1.0f - crop;
        filter->texcoords[7] = 0.0f;
    } else {
        filter->texcoords[0] = 0.0f;
        filter->texcoords[1] = 1.0f;
        filter->texcoords[2] = 1.0f;
        filter->texcoords[3] = 1.0f;
        filter->texcoords[4] = 0.0f;
        filter->texcoords[5] = 0.0f - crop;
        filter->texcoords[6] = 1.0f;
        filter->texcoords[7] = 0.0f - crop;
    }
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_fairytale *filter = opaque->filter;
    float bmp_aspect_ratio = (double)filter->xm_bmp[0].w / (double)filter->xm_bmp[0].h;
    float crop = 0.0f;
    float crop_h = 0.0f;
    if(bmp_aspect_ratio > filter->renderer->aspect_ratio)
    {
        crop = bmp_aspect_ratio * (double)filter->renderer->h;
        crop = (crop - (double)filter->renderer->w) / crop;
        XM_Fairytale_TexCoords_reset(filter, crop , true);
    } else {
        crop = (1.0f / bmp_aspect_ratio) * (double)filter->renderer->w;
        crop = (crop - (double)filter->renderer->h) / crop;
        XM_Fairytale_TexCoords_reset(filter, crop, false);
    }

    //XM_Filter_TexCoords_Rotation(filter->texcoords, NORMAL, false, true);
    glVertexAttribPointer(filter->av2_texcoord_1, 2, GL_FLOAT, GL_FALSE, 0, filter->texcoords);   XM_GLES2_checkError_TRACE("glVertexAttribPointer(av2_texcoord_1)");
    glEnableVertexAttribArray(filter->av2_texcoord_1);
}

static bool yuv420p_uploadTexture(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    if (!opaque || !opaque->filter)
        return 0;

    XM_BFilter_fairytale *filter = opaque->filter;

    glBindTexture(GL_TEXTURE_2D, filter->textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, filter->xm_bmp[0].w, filter->xm_bmp[0].h,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, filter->xm_bmp[0].pBitmap);
    return true;
}

//yuv420p_use must return 1
static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    if (!opaque || !opaque->filter)
        return 0;

    XM_BFilter_fairytale *filter = opaque->filter;

    if (0 == filter->textures[0])
        glGenTextures(1, filter->textures);

    glActiveTexture(GL_TEXTURE0 + textures_num);
    glBindTexture(GL_TEXTURE_2D, filter->textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, filter->xm_bmp[0].w, filter->xm_bmp[0].h,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, filter->xm_bmp[0].pBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glUniform1i(filter->us2_sampler_1, textures_num);
    return 1;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_fairytale *filter = opaque->filter;
    filter->us2_sampler_1 = glGetUniformLocation(filter->renderer->program, "us2_Sampler_1"); XM_GLES2_checkError_TRACE("glGetUniformLocation(us2_Sampler_1)");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_fairytale *filter = opaque->filter;
    filter->av2_texcoord_1 = glGetAttribLocation(filter->renderer->program, "av2_Texcoord_1");                XM_GLES2_checkError_TRACE("glGetAttribLocation(av2_Texcoord_1)");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_fairytale *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static void setBitmap(XM_Filter_Opaque *opaque, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_fairytale *filter = opaque->filter;
    filter->xm_bmp = xm_bmp[V_BASE_EFFECTS_TYPE_FAIRYTALE];
}

static const char *getVertexShader()
{
    return XM_GLES2_BaseFilter_getVertexShader_fairytale();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_fairytale();
}

static void XM_BFilter_reset(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_fairytale *filter = opaque->filter;
    for (int i = 0; i < 1; ++i) {
        if (filter->textures[i]) {
            glDeleteTextures(1, &filter->textures[i]);
            filter->textures[i] = 0;
        }
    }
}

static void XM_BFilter_Fairytale_free(XM_BFilter_fairytale *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_fairytale));
    free(filter);
}

static void XM_BFilter_Fairytale_freep(XM_BFilter_fairytale **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_Fairytale_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_Fairytale_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_Fairytale_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_fairytale *)calloc(1, sizeof(XM_BFilter_fairytale));

    filter->func_free = XM_BFilter_free;
    filter->func_reset = XM_BFilter_reset;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_setBitmap = setBitmap;
    filter->func_VertexShader_init = vsh_init;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_yuv420p_use = yuv420p_use;
    filter->func_yuv420p_uploadTexture= yuv420p_uploadTexture;
    filter->func_ShaderVariables_reset = shaderVariables_reset;
    filter->func_setShaderVariables = setShaderVariables;

    return filter;
}


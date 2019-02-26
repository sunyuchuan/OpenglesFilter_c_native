/*
* Created by sunyc on 2018-8-22.
* base filter freud.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_freud
{
    XM_GLES2_Base_Renderer *renderer;
    XM_Bitmap *xm_bmp;
    GLuint sampler2D[1];
    GLuint textures[1];
    GLuint strength;
    GLuint inputImageTextureHeight;
    GLuint inputImageTextureWidth;
} XM_BFilter_freud;

typedef struct XM_Filter_Opaque {
    XM_BFilter_freud *filter;
} XM_Filter_Opaque;

static void load_texture(XM_BFilter_freud *filter, int textures_num, int i, int w, int h, GLubyte *pixels)
{
    glActiveTexture(GL_TEXTURE0 + textures_num + i);
    glBindTexture(GL_TEXTURE_2D, filter->textures[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
                                0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glUniform1i(filter->sampler2D[i], textures_num + i);
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_freud *filter = opaque->filter;
    glUniform1f(filter->strength, 1.0f);
    glUniform1f(filter->inputImageTextureWidth, (float)filter->renderer->w);
    glUniform1f(filter->inputImageTextureHeight, (float)filter->renderer->h);
}

static bool yuv420p_uploadTexture(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    if (!opaque || !opaque->filter)
        return false;

    XM_BFilter_freud *filter = opaque->filter;

    for(int i = 0; i < 1; i++)
    {
        glBindTexture(GL_TEXTURE_2D, filter->textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, filter->xm_bmp[i].w, filter->xm_bmp[i].h,
                            0, GL_RGBA, GL_UNSIGNED_BYTE, filter->xm_bmp[i].pBitmap);
    }
    return true;
}

//yuv420p_use must return true
static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    if (!opaque || !opaque->filter)
        return 0;

    XM_BFilter_freud *filter = opaque->filter;

    if (0 == filter->textures[0])
        glGenTextures(1, filter->textures);

    load_texture(filter, textures_num, 0, filter->xm_bmp[0].w, filter->xm_bmp[0].h, filter->xm_bmp[0].pBitmap);

    return 1;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_freud *filter = opaque->filter;
    filter->sampler2D[0] = glGetUniformLocation(filter->renderer->program, "inputImageTexture2"); XM_GLES2_checkError_TRACE("glGetUniformLocation(inputImageTexture2)");
    filter->inputImageTextureHeight= glGetUniformLocation(filter->renderer->program, "inputImageTextureHeight");
    filter->inputImageTextureWidth= glGetUniformLocation(filter->renderer->program, "inputImageTextureWidth");
    filter->strength = glGetUniformLocation(filter->renderer->program, "strength");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
}

static void setBitmap(XM_Filter_Opaque *opaque, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_freud *filter = opaque->filter;
    filter->xm_bmp = xm_bmp[V_BASE_EFFECTS_TYPE_FREUD];
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_freud *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_freud();
}

static void XM_BFilter_reset(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_freud *filter = opaque->filter;
    for (int i = 0; i < 1; ++i) {
        if (filter->textures[i]) {
            glDeleteTextures(1, &filter->textures[i]);
            filter->textures[i] = 0;
        }
    }
}

static void XM_BFilter_Freud_free(XM_BFilter_freud *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_freud));
    free(filter);
}

static void XM_BFilter_Freud_freep(XM_BFilter_freud **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_Freud_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_Freud_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_Freud_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_freud *)calloc(1, sizeof(XM_BFilter_freud));

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


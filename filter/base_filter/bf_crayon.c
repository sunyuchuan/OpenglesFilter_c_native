/*
* Created by sunyc on 2018-8-22.
* base filter crayon.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_crayon
{
    XM_GLES2_Base_Renderer *renderer;
    XM_Bitmap *xm_bmp;
    GLuint singleStepOffset;
    GLuint strength;
} XM_BFilter_crayon;

typedef struct XM_Filter_Opaque {
    XM_BFilter_crayon *filter;
} XM_Filter_Opaque;

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_crayon *filter = opaque->filter;
    float offset[2] = {1.0f / (double)filter->renderer->w, 1.0f / (double)filter->renderer->h};
    glUniform1f(filter->strength, 0.5f);
    glUniform2fv(filter->singleStepOffset, 1, offset);
}

static bool yuv420p_uploadTexture(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    return true;
}

//yuv420p_use must return true
static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    return 1;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_crayon *filter = opaque->filter;
    filter->singleStepOffset = glGetUniformLocation(filter->renderer->program, "singleStepOffset");
    filter->strength = glGetUniformLocation(filter->renderer->program, "strength");
    glUniform1f(filter->strength, 2.0f);
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
}

static void setBitmap(XM_Filter_Opaque *opaque, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_crayon *filter = opaque->filter;
    filter->xm_bmp = xm_bmp[V_BASE_EFFECTS_TYPE_CRAYON];
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_crayon *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_crayon();
}

static void XM_BFilter_reset(XM_Filter_Opaque *opaque)
{
}

static void XM_BFilter_Crayon_free(XM_BFilter_crayon *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_crayon));
    free(filter);
}

static void XM_BFilter_Crayon_freep(XM_BFilter_crayon **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_Crayon_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_Crayon_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_Crayon_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_crayon *)calloc(1, sizeof(XM_BFilter_crayon));

    filter->func_free = XM_BFilter_free;
    filter->func_reset = XM_BFilter_reset;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    //filter->func_setBitmap = setBitmap;
    filter->func_VertexShader_init = vsh_init;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_yuv420p_use = yuv420p_use;
    filter->func_yuv420p_uploadTexture= yuv420p_uploadTexture;
    filter->func_ShaderVariables_reset = shaderVariables_reset;
    filter->func_setShaderVariables = setShaderVariables;

    return filter;
}


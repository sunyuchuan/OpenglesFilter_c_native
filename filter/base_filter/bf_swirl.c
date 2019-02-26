/*
* Created by sunyc on 2018-8-22.
* base filter swirl.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_swirl
{
    XM_GLES2_Base_Renderer *renderer;
    XM_Bitmap *xm_bmp;
    GLuint radius;
    GLuint angle;
    GLuint center;
} XM_BFilter_swirl;

typedef struct XM_Filter_Opaque {
    XM_BFilter_swirl *filter;
} XM_Filter_Opaque;

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_swirl *filter = opaque->filter;

    GLfloat color_ratio[2] = {0.5f, 0.5f};
    glUniform2fv(filter->center, 1, color_ratio);
    glUniform1f(filter->radius, 0.5f);
    glUniform1f(filter->angle, 1.0f);
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

    XM_BFilter_swirl *filter = opaque->filter;
    filter->radius = glGetUniformLocation(filter->renderer->program, "radius"); XM_GLES2_checkError_TRACE("glGetUniformLocation(radius)");
    filter->angle = glGetUniformLocation(filter->renderer->program, "angle"); XM_GLES2_checkError_TRACE("glGetUniformLocation(angle)");
    filter->center = glGetUniformLocation(filter->renderer->program, "center"); XM_GLES2_checkError_TRACE("glGetUniformLocation(center)");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
}

static void setBitmap(XM_Filter_Opaque *opaque, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_swirl *filter = opaque->filter;
    filter->xm_bmp = xm_bmp[V_BASE_EFFECTS_TYPE_SWIRL];
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_swirl *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_swirl();
}

static void XM_BFilter_reset(XM_Filter_Opaque *opaque)
{
}

static void XM_BFilter_Swirl_free(XM_BFilter_swirl *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_swirl));
    free(filter);
}

static void XM_BFilter_Swirl_freep(XM_BFilter_swirl **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_Swirl_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_Swirl_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_Swirl_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_swirl *)calloc(1, sizeof(XM_BFilter_swirl));

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


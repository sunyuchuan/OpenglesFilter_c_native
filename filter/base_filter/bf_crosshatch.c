/*
* Created by sunyc on 2018-8-22.
* base filter cross-hatch.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_crosshatch
{
    XM_GLES2_Base_Renderer *renderer;
    GLuint crossHatchSpacing;
    GLuint lineWidth;
} XM_BFilter_crosshatch;

typedef struct XM_Filter_Opaque {
    XM_BFilter_crosshatch *filter;
} XM_Filter_Opaque;

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_crosshatch *filter = opaque->filter;
    glUniform1f(filter->crossHatchSpacing, 0.03f);
    glUniform1f(filter->lineWidth, 0.003f);
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_crosshatch *filter = opaque->filter;
    filter->crossHatchSpacing = glGetUniformLocation(filter->renderer->program, "crossHatchSpacing"); XM_GLES2_checkError_TRACE("glGetUniformLocation(curve)");
    filter->lineWidth = glGetUniformLocation(filter->renderer->program, "lineWidth");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_crosshatch *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_crosshatch();
}

static void XM_BFilter_CrossHatch_free(XM_BFilter_crosshatch *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_crosshatch));
    free(filter);
}

static void XM_BFilter_CrossHatch_freep(XM_BFilter_crosshatch **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_CrossHatch_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_CrossHatch_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_CrossHatch_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_crosshatch *)calloc(1, sizeof(XM_BFilter_crosshatch));

    filter->func_free = XM_BFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_ShaderVariables_reset = shaderVariables_reset;

    return filter;
}


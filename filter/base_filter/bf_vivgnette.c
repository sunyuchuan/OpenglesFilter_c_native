/*
* Created by sunyc on 2018-8-22.
* base filter vivgnette.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_vivgnette
{
    XM_GLES2_Base_Renderer *renderer;
    GLuint vignetteCenter;
    GLuint vignetteColor;
    GLuint vignetteStart;
    GLuint vignetteEnd;
} XM_BFilter_vivgnette;

typedef struct XM_Filter_Opaque {
    XM_BFilter_vivgnette *filter;
} XM_Filter_Opaque;

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_vivgnette *filter = opaque->filter;

    float center[2] = {0.5f, 0.5f};
    float color[3] = {0.0f, 0.0f, 0.0f};
    glUniform2fv(filter->vignetteCenter, 1, center);
    glUniform3fv(filter->vignetteColor, 1, color);
    glUniform1f(filter->vignetteStart, 0.3f);
    glUniform1f(filter->vignetteEnd, 0.75f);
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_vivgnette *filter = opaque->filter;
    filter->vignetteCenter = glGetUniformLocation(filter->renderer->program, "vignetteCenter");
    filter->vignetteColor = glGetUniformLocation(filter->renderer->program, "vignetteColor");
    filter->vignetteStart = glGetUniformLocation(filter->renderer->program, "vignetteStart");
    filter->vignetteEnd = glGetUniformLocation(filter->renderer->program, "vignetteEnd");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_vivgnette *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_vivgnette();
}

static void XM_BFilter_Vivgnette_free(XM_BFilter_vivgnette *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_vivgnette));
    free(filter);
}

static void XM_BFilter_Vivgnette_freep(XM_BFilter_vivgnette **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_Vivgnette_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_Vivgnette_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_Vivgnette_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_vivgnette *)calloc(1, sizeof(XM_BFilter_vivgnette));

    filter->func_free = XM_BFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_ShaderVariables_reset = shaderVariables_reset;

    return filter;
}


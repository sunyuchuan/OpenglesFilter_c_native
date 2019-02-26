/*
* Created by sunyc on 2018-8-22.
* base filter sobel edge detection.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_edgeDetection
{
    XM_GLES2_Base_Renderer *renderer;
    GLuint texelWidth;
    GLuint texelHeight;
} XM_BFilter_edgeDetection;

typedef struct XM_Filter_Opaque {
    XM_BFilter_edgeDetection *filter;
} XM_Filter_Opaque;

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_edgeDetection *filter = opaque->filter;
    glUniform1f(filter->texelWidth, 1.0f / (double)filter->renderer->w);
    glUniform1f(filter->texelHeight, 1.0f / (double)filter->renderer->h);
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_edgeDetection *filter = opaque->filter;
    filter->texelWidth = glGetUniformLocation(filter->renderer->program, "texelWidth");
    filter->texelHeight = glGetUniformLocation(filter->renderer->program, "texelHeight");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_edgeDetection *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_BaseFilter_getVertexShader_3x3texture();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_edge_detection();
}

static void XM_BFilter_EdgeDetection_free(XM_BFilter_edgeDetection *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_edgeDetection));
    free(filter);
}

static void XM_BFilter_EdgeDetection_freep(XM_BFilter_edgeDetection **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_EdgeDetection_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_EdgeDetection_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_EdgeDetection_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_edgeDetection *)calloc(1, sizeof(XM_BFilter_edgeDetection));

    filter->func_free = XM_BFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_VertexShader_init = vsh_init;
    filter->func_ShaderVariables_reset = shaderVariables_reset;

    return filter;
}


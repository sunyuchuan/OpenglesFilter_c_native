/*
* Created by sunyc on 2018-8-22.
* base filter glass_sphere.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_glassSphere
{
    XM_GLES2_Base_Renderer *renderer;
    GLuint center;
    GLuint radius;
    GLuint aspectRatio;
    GLuint refractiveIndex;
} XM_BFilter_glassSphere;

typedef struct XM_Filter_Opaque {
    XM_BFilter_glassSphere *filter;
} XM_Filter_Opaque;

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_glassSphere *filter = opaque->filter;

    float center[2] = {0.5, 0.5};
    glUniform2fv(filter->center, 1, center);
    glUniform1f(filter->radius, 0.25);
    glUniform1f(filter->refractiveIndex, 0.71);
    glUniform1f(filter->aspectRatio, (double)filter->renderer->h / (double)filter->renderer->w);
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_glassSphere *filter = opaque->filter;
    filter->center = glGetUniformLocation(filter->renderer->program, "center");
    filter->radius = glGetUniformLocation(filter->renderer->program, "radius");
    filter->aspectRatio = glGetUniformLocation(filter->renderer->program, "aspectRatio");
    filter->refractiveIndex = glGetUniformLocation(filter->renderer->program, "refractiveIndex");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_glassSphere *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_glass_sphere();
}

static void XM_BFilter_GlassSphere_free(XM_BFilter_glassSphere *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_glassSphere));
    free(filter);
}

static void XM_BFilter_GlassSphere_freep(XM_BFilter_glassSphere **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_GlassSphere_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_GlassSphere_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_GlassSphere_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_glassSphere *)calloc(1, sizeof(XM_BFilter_glassSphere));

    filter->func_free = XM_BFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_ShaderVariables_reset = shaderVariables_reset;

    return filter;
}


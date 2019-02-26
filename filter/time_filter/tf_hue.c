/*
* Created by sunyc on 2018-8-22.
* time filter hue.
*/
#include "ijksdl/gles2_xm/internal.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"

typedef struct XM_TFilter_hue
{
    XM_GLES2_Renderer *renderer;
    GLuint hueAdjust;
} XM_TFilter_hue;

typedef struct XM_Filter_Opaque {
    XM_TFilter_hue *filter;
} XM_Filter_Opaque;

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_hue *filter = opaque->filter;
    float hueAdjust = (90.0f * 3.1415926) / 180.0f;
    glUniform1f(filter->hueAdjust, hueAdjust);
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_hue *filter = opaque->filter;
    filter->hueAdjust = glGetUniformLocation(filter->renderer->program, "hueAdjust");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_hue *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_TFilter_yuv420p_getFragmentShader_hue();
}

static void XM_TFilter_Hue_free(XM_TFilter_hue *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_TFilter_hue));
    free(filter);
}

static void XM_TFilter_Hue_freep(XM_TFilter_hue **filter)
{
    if (!filter || !*filter)
        return;

    XM_TFilter_Hue_free(*filter);
    *filter = NULL;
}

static void XM_TFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_Hue_freep(&opaque->filter);
}

XM_Filter *XM_TFilter_Hue_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_TFilter_hue *)calloc(1, sizeof(XM_TFilter_hue));

    filter->func_free = XM_TFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_ShaderVariables_reset = shaderVariables_reset;

    return filter;
}


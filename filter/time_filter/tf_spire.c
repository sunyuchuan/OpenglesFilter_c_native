/*
* Created by sunyc on 2018-8-22.
* time filter spire.
*/
#include "ijksdl/gles2_xm/internal.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"

typedef struct XM_TFilter_spire
{
    XM_GLES2_Renderer *renderer;
    GLuint radius;
    GLuint angle;
    GLuint center;
} XM_TFilter_spire;

typedef struct XM_Filter_Opaque {
    XM_TFilter_spire *filter;
} XM_Filter_Opaque;

static void XM_Filter_Spire_reload(XM_TFilter_spire *filter)
{
    GLfloat color_ratio[2] = {0.5f, 0.5f};

    glUniform2fv(filter->center, 1, color_ratio);
    glUniform1f(filter->radius, 0.5f);
    glUniform1f(filter->angle, 1.0f);
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_spire *filter = opaque->filter;
    XM_Filter_Spire_reload(filter);
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_spire *filter = opaque->filter;
    filter->radius = glGetUniformLocation(filter->renderer->program, "radius"); XM_GLES2_checkError_TRACE("glGetUniformLocation(radius)");
    filter->angle = glGetUniformLocation(filter->renderer->program, "angle"); XM_GLES2_checkError_TRACE("glGetUniformLocation(angle)");
    filter->center = glGetUniformLocation(filter->renderer->program, "center"); XM_GLES2_checkError_TRACE("glGetUniformLocation(center)");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_spire *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_TFilter_yuv420p_getFragmentShader_spire();
}

static void XM_TFilter_Spire_free(XM_TFilter_spire *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_TFilter_spire));
    free(filter);
}

static void XM_TFilter_Spire_freep(XM_TFilter_spire **filter)
{
    if (!filter || !*filter)
        return;

    XM_TFilter_Spire_free(*filter);
    *filter = NULL;
}

static void XM_TFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_Spire_freep(&opaque->filter);
}

XM_Filter *XM_TFilter_Spire_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_TFilter_spire *)calloc(1, sizeof(XM_TFilter_spire));

    filter->func_free = XM_TFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_ShaderVariables_reset = shaderVariables_reset;

    return filter;
}


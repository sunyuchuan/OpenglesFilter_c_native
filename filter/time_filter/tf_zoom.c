/*
* Created by sunyc on 2018-8-22.
* time filter zoom.
*/
#include "ijksdl/gles2_xm/internal.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"

typedef struct XM_TFilter_zoom
{
    XM_GLES2_Renderer *renderer;
    GLuint vv3_dot;
    GLuint uXY;
} XM_TFilter_zoom;

typedef struct XM_Filter_Opaque {
    XM_TFilter_zoom *filter;
} XM_Filter_Opaque;

static void XM_Filter_Zoom_reload(XM_TFilter_zoom *filter, XM_Filter_ShaderParameter *param)
{
    GLfloat color_ratio[3] = {0.1, 0.6, 0.2};
    if(param->off_screen)
    {
        color_ratio[1] = -color_ratio[1];
    }
    if(!isnan(filter->renderer->aspect_ratio))
    {
        glUniform3fv(filter->vv3_dot, 1, color_ratio);
        glUniform1f(filter->uXY, filter->renderer->aspect_ratio);
    }
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_zoom *filter = opaque->filter;
    XM_Filter_Zoom_reload(filter, param);
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_zoom *filter = opaque->filter;
    filter->vv3_dot = glGetUniformLocation(filter->renderer->program, "vv3_dot"); XM_GLES2_checkError_TRACE("glGetUniformLocation(vv3_dot)");
    filter->uXY = glGetUniformLocation(filter->renderer->program, "uXY"); XM_GLES2_checkError_TRACE("glGetUniformLocation(uXY)");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_zoom *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_TimeFilter_getVertexShader_zoom();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_TFilter_yuv420p_getFragmentShader_zoom();
}

static void XM_TFilter_Zoom_free(XM_TFilter_zoom *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_TFilter_zoom));
    free(filter);
}

static void XM_TFilter_Zoom_freep(XM_TFilter_zoom **filter)
{
    if (!filter || !*filter)
        return;

    XM_TFilter_Zoom_free(*filter);
    *filter = NULL;
}

static void XM_TFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_Zoom_freep(&opaque->filter);
}

XM_Filter *XM_TFilter_Zoom_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_TFilter_zoom *)calloc(1, sizeof(XM_TFilter_zoom));

    filter->func_free = XM_TFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_ShaderVariables_reset = shaderVariables_reset;

    return filter;
}


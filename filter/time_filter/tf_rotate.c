/*
* Created by sunyc on 2018-8-21.
* time filter rotate.
*/
#include "ijksdl/gles2_xm/internal.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"

typedef struct XM_TFilter_rotate
{
    GLuint um4_rotate;
    GLfloat scale;
    GLfloat step;
    XM_GLES2_Renderer *renderer;
} XM_TFilter_rotate;

typedef struct XM_Filter_Opaque {
    XM_TFilter_rotate *filter;
} XM_Filter_Opaque;

static void XM_Filter_Rotate_reload(XM_TFilter_rotate *filter, GLfloat angle, GLfloat aspect, GLfloat scale)
{
    XM_GLES_Matrix rotateMatrix;
    XM_GLES2_loadRotateMatrix(&rotateMatrix, angle, aspect, scale);
    glUniformMatrix4fv(filter->um4_rotate, 1, GL_FALSE, rotateMatrix.m);                    XM_GLES2_checkError_TRACE("glUniformMatrix4fv(um4_rotate)");
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_rotate *filter = opaque->filter;
    if(filter->step <= 360.0f) {
        filter->step += 7.0f;
        filter->scale += 7.0f / 360.0f;
    } else {
        filter->step = 7.0f;
        filter->scale = 0.0f;
    }
    XM_Filter_Rotate_reload(filter, filter->step, filter->renderer->aspect_ratio, filter->scale);
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_Filter_Rotate_reload(opaque->filter, 0.0f, 0.0f, 0.0f);
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter || !opaque->filter ->renderer)
        return;

    XM_TFilter_rotate *filter = opaque->filter;
    filter->um4_rotate = glGetUniformLocation(filter->renderer->program, "um4_Rotate");	XM_GLES2_checkError_TRACE("glGetUniformLocation(um4_Rotate)");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_rotate *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_TimeFilter_getVertexShader_rotate();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_GLES2_getDefaultFragmentShader_yuv420p();
}

static void XM_TFilter_Rotate_free(XM_TFilter_rotate *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Rotate_terminate(filter);
    memset(filter, 0, sizeof(XM_TFilter_rotate));
    free(filter);
}

static void XM_TFilter_Rotate_freep(XM_TFilter_rotate **filter)
{
    if (!filter || !*filter)
        return;

    XM_TFilter_Rotate_free(*filter);
    *filter = NULL;
}

static void XM_TFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_Rotate_freep(&opaque->filter);
}

XM_Filter *XM_TFilter_Rotate_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_TFilter_rotate *)calloc(1, sizeof(XM_TFilter_rotate));

    filter->func_free = XM_TFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_VertexShader_init = vsh_init;
    filter->func_ShaderVariables_reset = shaderVariables_reset;
    filter->func_setShaderVariables = setShaderVariables;

    return filter;
}


/*
* Created by sunyc on 2018-8-22.
* base filter laplacian.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_laplacian
{
    XM_GLES2_Base_Renderer *renderer;
    XM_Bitmap *xm_bmp;
    GLuint convolutionMatrix;
    GLuint texelWidth;
    GLuint texelHeight;
} XM_BFilter_laplacian;

typedef struct XM_Filter_Opaque {
    XM_BFilter_laplacian *filter;
} XM_Filter_Opaque;

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_laplacian *filter = opaque->filter;
    float m[9] = {
                0.5f, 1.0f, 0.5f,
                1.0f, -6.0f, 1.0f,
                0.5f, 1.0f, 0.5f
    };
    glUniform1f(filter->texelWidth, 1.0f / (double)filter->renderer->w);
    glUniform1f(filter->texelHeight, 1.0f / (double)filter->renderer->h);
    glUniformMatrix3fv(filter->convolutionMatrix, 1, GL_FALSE, m);
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_laplacian *filter = opaque->filter;
    filter->convolutionMatrix = glGetUniformLocation(filter->renderer->program, "convolutionMatrix");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_laplacian *filter = opaque->filter;
    filter->texelWidth = glGetUniformLocation(filter->renderer->program, "texelWidth");
    filter->texelHeight = glGetUniformLocation(filter->renderer->program, "texelHeight");
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_laplacian *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_BaseFilter_getVertexShader_3x3texture();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_laplacian();
}

static void XM_BFilter_Laplacian_free(XM_BFilter_laplacian *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_laplacian));
    free(filter);
}

static void XM_BFilter_Laplacian_freep(XM_BFilter_laplacian **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_Laplacian_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_Laplacian_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_Laplacian_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_laplacian *)calloc(1, sizeof(XM_BFilter_laplacian));

    filter->func_free = XM_BFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_VertexShader_init = vsh_init;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_ShaderVariables_reset = shaderVariables_reset;

    return filter;
}


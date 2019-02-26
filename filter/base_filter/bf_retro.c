/*
* Created by sunyc on 2018-8-22.
* base filter retro.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_retro
{
    XM_GLES2_Base_Renderer *renderer;
    GLuint vv3_color_ratio;
    GLfloat color_ratio[3];
} XM_BFilter_retro;

typedef struct XM_Filter_Opaque {
    XM_BFilter_retro *filter;
} XM_Filter_Opaque;

static void XM_Filter_Retro_reload(XM_BFilter_retro *filter)
{
    filter->color_ratio[0] = 0.2;
    filter->color_ratio[1] = 0.3;
    filter->color_ratio[2] = 0.5;
    glUniform3fv(filter->vv3_color_ratio, 1, filter->color_ratio);
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_retro *filter = opaque->filter;
    XM_Filter_Retro_reload(filter);
}

static bool yuv420p_uploadTexture(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    return true;
}

static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    return 1;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_retro *filter = opaque->filter;
    filter->vv3_color_ratio = glGetUniformLocation(filter->renderer->program, "vv3_ColorRatio"); XM_GLES2_checkError_TRACE("glGetUniformLocation(vv3_ColorRatio)");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_retro *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_BaseFilter_getVertexShader_retro();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_retro();
}

static void XM_BFilter_reset(XM_Filter_Opaque *opaque)
{
}

static void XM_BFilter_Retro_free(XM_BFilter_retro *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_retro));
    free(filter);
}

static void XM_BFilter_Retro_freep(XM_BFilter_retro **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_Retro_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_Retro_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_Retro_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_retro *)calloc(1, sizeof(XM_BFilter_retro));

    filter->func_free = XM_BFilter_free;
    filter->func_reset = XM_BFilter_reset;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_VertexShader_init = vsh_init;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_yuv420p_use = yuv420p_use;
    filter->func_yuv420p_uploadTexture= yuv420p_uploadTexture;
    filter->func_ShaderVariables_reset = shaderVariables_reset;
    filter->func_setShaderVariables = setShaderVariables;

    return filter;
}


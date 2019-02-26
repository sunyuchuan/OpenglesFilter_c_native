/*
* Created by sunyc on 2018-8-22.
* time filter retro.
*/
#include "ijksdl/gles2_xm/internal.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"

typedef struct XM_TFilter_sphere
{
    XM_GLES2_Renderer *renderer;
    GLuint center;
    GLuint radius;
    GLuint aspectRatio;
    GLuint refractiveIndex;
} XM_TFilter_sphere;

typedef struct XM_Filter_Opaque {
    XM_TFilter_sphere *filter;
} XM_Filter_Opaque;

static void XM_Filter_Sphere_reload(XM_TFilter_sphere *filter)
{
    GLfloat center[2] = {0.5f, 0.5f};
    glUniform2fv(filter->center, 1, center);
    glUniform1f(filter->radius, 0.25f);
    glUniform1f(filter->aspectRatio, 1.0f / filter->renderer->aspect_ratio);
    glUniform1f(filter->refractiveIndex, 0.71f);
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_sphere *filter = opaque->filter;
    XM_Filter_Sphere_reload(filter);
    XM_Filter_TexCoords_Rotation(filter->renderer->texcoords, ROTATION_180, true, false);
}

static bool yuv420p_uploadTexture(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    return true;
}

static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    return 0;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_sphere *filter = opaque->filter;
    filter->center = glGetUniformLocation(filter->renderer->program, "center"); XM_GLES2_checkError_TRACE("glGetUniformLocation(center)");
    filter->radius = glGetUniformLocation(filter->renderer->program, "radius"); XM_GLES2_checkError_TRACE("glGetUniformLocation(radius)");
    filter->aspectRatio = glGetUniformLocation(filter->renderer->program, "aspectRatio"); XM_GLES2_checkError_TRACE("glGetUniformLocation(aspectRatio)");
    filter->refractiveIndex = glGetUniformLocation(filter->renderer->program, "refractiveIndex"); XM_GLES2_checkError_TRACE("glGetUniformLocation(refractiveIndex)");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_sphere *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_TFilter_yuv420p_getFragmentShader_sphere();
}

static void XM_TFilter_reset(XM_Filter_Opaque *opaque)
{
}

static void XM_TFilter_Sphere_free(XM_TFilter_sphere *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_TFilter_sphere));
    free(filter);
}

static void XM_TFilter_Sphere_freep(XM_TFilter_sphere **filter)
{
    if (!filter || !*filter)
        return;

    XM_TFilter_Sphere_free(*filter);
    *filter = NULL;
}

static void XM_TFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_Sphere_freep(&opaque->filter);
}

XM_Filter *XM_TFilter_Sphere_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_TFilter_sphere *)calloc(1, sizeof(XM_TFilter_sphere));

    filter->func_free = XM_TFilter_free;
    filter->func_reset = XM_TFilter_reset;
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


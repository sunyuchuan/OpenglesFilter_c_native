/*
* Created by sunyc on 2018-8-22.
* time filter fission.
*/
#include "ijksdl/gles2_xm/internal.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"

typedef struct XM_TFilter_fission
{
    XM_GLES2_Renderer *renderer;
    GLuint step;
    GLuint num;
    GLfloat texcoords[8];
} XM_TFilter_fission;

typedef struct XM_Filter_Opaque {
    XM_TFilter_fission *filter;
} XM_Filter_Opaque;

static void XM_Filter_Fission_reload(XM_TFilter_fission *filter, int num)
{
    for(int i = 0; i < 8; i++)
    {
        filter->texcoords[i] = filter->renderer->texcoords[i] * num;
    }
    glVertexAttribPointer(filter->renderer->av2_texcoord, 2, GL_FLOAT, GL_FALSE, 0, filter->texcoords);   XM_GLES2_checkError_TRACE("glVertexAttribPointer(av2_texcoord)");
    glEnableVertexAttribArray(filter->renderer->av2_texcoord);
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;
    XM_TFilter_fission *filter = opaque->filter;

    if(filter->step < 25)
    {
        filter->step ++;
        filter->num = 2;
    } else if(filter->step < 25*2) {
        filter->step ++;
        filter->num = 3;
    } else {
        filter->step = 0;
        filter->num = 2;
    }
    XM_Filter_Fission_reload(filter, filter->num);
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_fission *filter = opaque->filter;
    XM_Filter_Fission_reload(filter, 2);
}

static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    if (!opaque || !opaque->filter)
        return 0;

    XM_TFilter_fission *filter = opaque->filter;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glUseProgram(filter->renderer->program);            XM_GLES2_checkError_TRACE("glUseProgram");

    for (int i = 0; i < 3; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, filter->renderer->plane_textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    return 0;
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_fission *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_GLES2_getDefaultFragmentShader_yuv420p();
}

static void XM_TFilter_Fission_free(XM_TFilter_fission *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_TFilter_fission));
    free(filter);
}

static void XM_TFilter_Fission_freep(XM_TFilter_fission **filter)
{
    if (!filter || !*filter)
        return;

    XM_TFilter_Fission_free(*filter);
    *filter = NULL;
}

static void XM_TFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_Fission_freep(&opaque->filter);
}

XM_Filter *XM_TFilter_Fission_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_TFilter_fission *)calloc(1, sizeof(XM_TFilter_fission));

    filter->func_free = XM_TFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_yuv420p_use = yuv420p_use;
    filter->func_ShaderVariables_reset = shaderVariables_reset;
    filter->func_setShaderVariables = setShaderVariables;

    return filter;
}


/*
* Created by sunyc on 2018-8-21.
* Abstraction and packaging about filters.
*/
#include "xm_filter.h"

XM_Filter *XM_Filter_create(size_t opaque_size)
{
    XM_Filter *filter = (XM_Filter *)calloc(1, sizeof(XM_Filter));
    if (!filter)
        return NULL;
    filter->v_effects.timeType_index = -1;
    filter->v_effects.baseType_index = -1;

    filter->opaque = calloc(1, opaque_size);
    if (!filter->opaque) {
        free(filter);
        return NULL;
    }

    return filter;
}

const char *XM_Filter_yuv420p_getFragmentShader(XM_Filter *filter)
{
    const char *fsh = NULL;
    if (!filter)
        return fsh;

    if (filter->func_yuv420p_getFragmentShader) {
        fsh = filter->func_yuv420p_getFragmentShader();
    }

    filter->fsh = fsh;
    return fsh;
}

const char *XM_Filter_getVertexShader(XM_Filter *filter)
{
    const char *vsh = NULL;
    if (!filter)
        return vsh;

    if (filter->func_getVertexShader) {
        vsh = filter->func_getVertexShader();
    }

    filter->vsh = vsh;
    return vsh;
}

void XM_Filter_setRenderer(XM_Filter *filter, void *renderer)
{
    if (!filter || !renderer)
        return;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_setRenderer) {
        filter->func_setRenderer(opaque, renderer);
    }
}

void XM_Filter_setBitmap(XM_Filter *filter, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!filter || !xm_bmp)
        return;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_setBitmap) {
        filter->func_setBitmap(opaque, xm_bmp);
    }
}

void XM_Filter_VertexShader_init(XM_Filter *filter)
{
    if (!filter)
        return;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_VertexShader_init) {
        filter->func_VertexShader_init(opaque);
    }
}

void XM_Filter_FragmentShader_init(XM_Filter *filter)
{
    if (!filter)
        return;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_FragmentShader_init) {
        filter->func_FragmentShader_init(opaque);
    }
}

int XM_Filter_yuv420p_use(XM_Filter *filter, int textures_num)
{
    if (!filter)
        return 0;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_yuv420p_use) {
        return filter->func_yuv420p_use(opaque, textures_num);
    }

    return 1;
}

bool XM_Filter_yuv420p_uploadTexture(XM_Filter *filter, SDL_VoutOverlay *overlay)
{
    if (!filter)
        return true;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_yuv420p_uploadTexture) {
        return filter->func_yuv420p_uploadTexture(opaque, overlay);
    }

    return true;
}

void XM_Filter_ShaderVariables_reset(XM_Filter *filter, XM_Filter_ShaderParameter *param)
{
    if (!filter || !param)
        return;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_ShaderVariables_reset) {
        filter->func_ShaderVariables_reset(opaque, param);
    }
}

void XM_Filter_setShaderVariables(XM_Filter *filter, XM_Filter_ShaderParameter *param)
{
    if (!filter || !param)
        return;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_setShaderVariables) {
        filter->func_setShaderVariables(opaque, param);
    }
}

void XM_Filter_free(XM_Filter *filter)
{
    if (!filter)
        return;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_free) {
        filter->func_free(opaque);
    }

    free(opaque);
    memset(filter, 0, sizeof(XM_Filter));
    free(filter);
}

void XM_Filter_freep(XM_Filter **filter)
{
    if (!filter || !*filter)
        return;

    XM_Filter_free(*filter);
    *filter = NULL;
}

void XM_Filter_reset(XM_Filter *filter)
{
    if (!filter)
        return;

    XM_Filter_Opaque *opaque = filter->opaque;
    if (filter->func_reset) {
        filter->func_reset(opaque);
    }
}


/*
* Created by sunyc on 2018-8-22.
* time filter invert.
*/
#include "ijksdl/gles2_xm/internal.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"

typedef struct XM_TFilter_invert
{
} XM_TFilter_invert;

typedef struct XM_Filter_Opaque {
    XM_TFilter_invert *filter;
} XM_Filter_Opaque;

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_TFilter_yuv420p_getFragmentShader_invert();
}

static void XM_TFilter_Invert_free(XM_TFilter_invert *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_TFilter_invert));
    free(filter);
}

static void XM_TFilter_Invert_freep(XM_TFilter_invert **filter)
{
    if (!filter || !*filter)
        return;

    XM_TFilter_Invert_free(*filter);
    *filter = NULL;
}

static void XM_TFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_TFilter_Invert_freep(&opaque->filter);
}

XM_Filter *XM_TFilter_Invert_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_TFilter_invert *)calloc(1, sizeof(XM_TFilter_invert));

    filter->func_free = XM_TFilter_free;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;

    return filter;
}


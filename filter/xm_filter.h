/*
* Created by sunyc on 2018-8-21.
* Abstraction and packaging about filters.
*/

#ifndef XM_FILTER_H
#define XM_FILTER_H
#include <stddef.h>
#include <stdbool.h>
#include "ijksdl/ijksdl_vout.h"
#include "ijksdl/xm_bitmap.h"
#include "ijksdl/xm_mtcnn_ncnn.h"

typedef struct XM_Filter_Opaque XM_Filter_Opaque;
typedef struct XM_GLES2_Renderer XM_GLES2_Renderer;

typedef struct XM_Filter_ShaderParameter
{
    bool off_screen;
    int face_rectangle_coordinates[FACE_DETECT_OUTPUT_LEN];
} XM_Filter_ShaderParameter;

typedef struct XM_Filter
{
    XM_Filter_Opaque *opaque;
    SDL_VideoEffects v_effects;
    const char *fsh;
    const char *vsh;

    void (*func_free)(XM_Filter_Opaque *opaque);
    void (*func_reset)(XM_Filter_Opaque *opaque);
    const char *(*func_yuv420p_getFragmentShader)();
    const char *(*func_getVertexShader)();
    void (*func_setRenderer)(XM_Filter_Opaque *opaque, void *renderer);
    void (*func_setBitmap)(XM_Filter_Opaque *opaque, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX]);
    void (*func_VertexShader_init)(XM_Filter_Opaque *opaque);
    void (*func_FragmentShader_init)(XM_Filter_Opaque *opaque);
    int (*func_yuv420p_use)(XM_Filter_Opaque *opaque, int textures_num);
    bool (*func_yuv420p_uploadTexture)(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay);
    void (*func_ShaderVariables_reset)(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param);
    void (*func_setShaderVariables)(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param);
} XM_Filter;

XM_Filter *XM_Filter_create(size_t opaque_size);
void XM_Filter_free(XM_Filter *filter);
void XM_Filter_freep(XM_Filter **filter);
void XM_Filter_reset(XM_Filter *filter);

const char *XM_Filter_yuv420p_getFragmentShader(XM_Filter *filter);
const char *XM_Filter_getVertexShader(XM_Filter *filter);
void XM_Filter_setRenderer(XM_Filter *filter, void *renderer);
void XM_Filter_setBitmap(XM_Filter *filter, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX]);
void XM_Filter_VertexShader_init(XM_Filter *filter);
void XM_Filter_FragmentShader_init(XM_Filter *filter);
int XM_Filter_yuv420p_use(XM_Filter *filter, int textures_num);
bool XM_Filter_yuv420p_uploadTexture(XM_Filter *filter, SDL_VoutOverlay *overlay);
void XM_Filter_ShaderVariables_reset(XM_Filter *filter, XM_Filter_ShaderParameter *param);
void XM_Filter_setShaderVariables(XM_Filter *filter, XM_Filter_ShaderParameter *param);

const char *XM_GLES2_getVertexShader_default();
const char *XM_GLES2_getDefaultFragmentShader_yuv420p();
const char *XM_GLES2_getFragmentShader_yuv444p10le();
const char *XM_GLES2_getFragmentShader_yuv420sp();
const char *XM_GLES2_getFragmentShader_rgb();

int XM_TFilter_getBitmapNumber(int index);
int XM_BFilter_getBitmapNumber(int index);
#endif

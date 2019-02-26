/*
* Created by sunyc on 2018-8-14.
* Abstraction and packaging about gles effects.
*/

#ifndef XM_TIME_FILTER_EFFECTS_H
#define XM_TIME_FILTER_EFFECTS_H

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif
#include "filter/xm_filter.h"

typedef struct XM_GLES2_Renderer XM_GLES2_Renderer;
typedef struct SDL_VoutOverlay SDL_VoutOverlay;
typedef struct SDL_VideoEffects SDL_VideoEffects;

const char *XM_GLES2_TimeFilter_getVertexShader_ghost();
const char *XM_GLES2_TimeFilter_getVertexShader_zoom();
const char *XM_GLES2_TimeFilter_getVertexShader_rotate();
const char *XM_GLES2_getVertexShader_face_sticker();

const char *XM_TFilter_yuv420p_getFragmentShader_ghost();
const char *XM_TFilter_yuv420p_getFragmentShader_retro();
const char *XM_TFilter_yuv420p_getFragmentShader_zoom();
const char *XM_TFilter_yuv420p_getFragmentShader_spire();
const char *XM_TFilter_yuv420p_getFragmentShader_sphere();
const char *XM_TFilter_yuv420p_getFragmentShader_invert();
const char *XM_TFilter_yuv420p_getFragmentShader_hue();
const char *XM_TFilter_yuv420p_getFragmentShader_face_sticker();

XM_Filter *XM_TFilter_Ghost_create();
XM_Filter *XM_TFilter_Retro_create();
XM_Filter *XM_TFilter_Rotate_create();
XM_Filter *XM_TFilter_Zoom_create();
XM_Filter *XM_TFilter_Fission_create();
XM_Filter *XM_TFilter_Spire_create();
XM_Filter *XM_TFilter_Sphere_create();
XM_Filter *XM_TFilter_Invert_create();
XM_Filter *XM_TFilter_Hue_create();
XM_Filter *XM_TFilter_FaceSticker_create();

int XM_TFilter_yuv420p_getPlanesTexturesNum(SDL_VideoEffects *veffects);
GLboolean XM_GLES2_TimeFilter_isValid(XM_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay);
#endif

/*
* Created by sunyc on 2018-8-14.
* Abstraction and packaging about gles base effects.
*/

#ifndef XM_BASE_FILTER_EFFECTS_H
#define XM_BASE_FILTER_EFFECTS_H

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif
#include "filter/xm_filter.h"

const char *XM_GLES2_BaseFilter_getVertexShader_retro();
const char *XM_GLES2_BaseFilter_getVertexShader_fairytale();
const char *XM_GLES2_BaseFilter_getVertexShader_3x3texture();
const char *XM_BFilter_getFragmentShader_retro();
const char *XM_BFilter_getFragmentShader_fairytale();
const char *XM_BFilter_getFragmentShader_blackcat();
const char *XM_BFilter_getFragmentShader_skinwhiten();
const char *XM_BFilter_getFragmentShader_sunrise();
const char *XM_BFilter_getFragmentShader_sunset();
const char *XM_BFilter_getFragmentShader_sweets();
const char *XM_BFilter_getFragmentShader_whitecat();
const char *XM_BFilter_getFragmentShader_antique();
const char *XM_BFilter_getFragmentShader_calm();
const char *XM_BFilter_getFragmentShader_cool();
const char *XM_BFilter_getFragmentShader_crayon();
const char *XM_BFilter_getFragmentShader_emerald();
const char *XM_BFilter_getFragmentShader_evergreen();
const char *XM_BFilter_getFragmentShader_latte();
const char *XM_BFilter_getFragmentShader_nostalgia();
const char *XM_BFilter_getFragmentShader_romance();
const char *XM_BFilter_getFragmentShader_sakura();
const char *XM_BFilter_getFragmentShader_sketch();
const char *XM_BFilter_getFragmentShader_tender();
const char *XM_BFilter_getFragmentShader_warm();
const char *XM_BFilter_getFragmentShader_amaro();
const char *XM_BFilter_getFragmentShader_brannan();
const char *XM_BFilter_getFragmentShader_brooklyn();
const char *XM_BFilter_getFragmentShader_earlybird();
const char *XM_BFilter_getFragmentShader_freud();
const char *XM_BFilter_getFragmentShader_hefe();
const char *XM_BFilter_getFragmentShader_hudson();
const char *XM_BFilter_getFragmentShader_inkwell();
const char *XM_BFilter_getFragmentShader_swirl();
const char *XM_BFilter_getFragmentShader_edge_detection();
const char *XM_BFilter_getFragmentShader_glass_sphere();
const char *XM_BFilter_getFragmentShader_crosshatch();
const char *XM_BFilter_getFragmentShader_laplacian();
const char *XM_BFilter_getFragmentShader_face_sticker();

XM_Filter *XM_BFilter_Retro_create();
XM_Filter *XM_BFilter_Fairytale_create();
XM_Filter *XM_BFilter_Blackcat_create();
XM_Filter *XM_BFilter_SkinWhiten_create();
XM_Filter *XM_BFilter_SunRise_create();
XM_Filter *XM_BFilter_SunSet_create();
XM_Filter *XM_BFilter_Sweets_create();
XM_Filter *XM_BFilter_WhiteCat_create();
XM_Filter *XM_BFilter_Antique_create();
XM_Filter *XM_BFilter_Calm_create();
XM_Filter *XM_BFilter_Cool_create();
XM_Filter *XM_BFilter_Crayon_create();
XM_Filter *XM_BFilter_Emerald_create();
XM_Filter *XM_BFilter_Evergreen_create();
XM_Filter *XM_BFilter_Latte_create();
XM_Filter *XM_BFilter_Nostalgia_create();
XM_Filter *XM_BFilter_Romance_create();
XM_Filter *XM_BFilter_Sakura_create();
XM_Filter *XM_BFilter_Sketch_create();
XM_Filter *XM_BFilter_Tender_create();
XM_Filter *XM_BFilter_Warm_create();
XM_Filter *XM_BFilter_Amaro_create();
XM_Filter *XM_BFilter_Brannan_create();
XM_Filter *XM_BFilter_Brooklyn_create();
XM_Filter *XM_BFilter_EarlyBird_create();
XM_Filter *XM_BFilter_Freud_create();
XM_Filter *XM_BFilter_Hefe_create();
XM_Filter *XM_BFilter_Hudson_create();
XM_Filter *XM_BFilter_Inkwell_create();
XM_Filter *XM_BFilter_Swirl_create();
XM_Filter *XM_BFilter_EdgeDetection_create();
XM_Filter *XM_BFilter_GlassSphere_create();
XM_Filter *XM_BFilter_CrossHatch_create();
XM_Filter *XM_BFilter_Laplacian_create();
XM_Filter *XM_BFilter_FaceSticker_create();

#endif

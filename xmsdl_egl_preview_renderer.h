/*
* Created by sunyc on 2018-8-9.
* Abstract ijkegl xmegl etc.
*/

#ifndef XM_SDL_EGL_PREVIEW_RENDERER_H
#define XM_SDL_EGL_PREVIEW_RENDERER_H

#include "ijksdl/sdl_egl.h"

typedef struct XM_EGL_Opaque XM_EGL_Opaque;

typedef struct XM_EGL
{
    SDL_Class      *opaque_class;
    XM_EGL_Opaque *opaque;

    EGLNativeWindowType window;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    EGLint width;
    EGLint height;
} XM_EGL;

SDL_EGL *XMSDL_EGL_preview_create();

#endif

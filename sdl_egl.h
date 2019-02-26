/*
* Created by sunyc on 2018-8-9.
* Abstract ijkegl xmegl etc.
*/

#ifndef SDL_EGL_H
#define SDL_EGL_H

#ifdef __APPLE__
#include "ijksdl/ios/EGL/egl.h"
#include "ijksdl/ios/EGL/eglplatform.h"
#else
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#endif
#include "ijksdl_class.h"
#include <stdbool.h>

//#define EGL_BITMAP_NUMBER_MAX 4

typedef struct SDL_VoutOverlay SDL_VoutOverlay;
typedef struct SDL_EGL_Opaque SDL_EGL_Opaque;
typedef struct SDL_XM_Filter_Bitmap SDL_XM_Filter_Bitmap;

typedef struct SDL_EGL {
    SDL_EGL_Opaque *opaque;

    bool (*func_display)(SDL_EGL_Opaque *opaque, EGLNativeWindowType window, SDL_VoutOverlay *overlay);
    void (*func_set_bitmap)(SDL_EGL_Opaque *opaque, SDL_XM_Filter_Bitmap *f_bitmap);
    void (*func_terminate)(SDL_EGL_Opaque *opaque);
    void (*func_free)(SDL_EGL_Opaque *opaque);
} SDL_EGL;

SDL_EGL *SDL_EGL_create(size_t opaque_size);
bool SDL_EGL_display(SDL_EGL *egl, EGLNativeWindowType window, SDL_VoutOverlay *overlay);
void SDL_EGL_set_bitmap(SDL_EGL *egl, SDL_XM_Filter_Bitmap *f_bitmap);
void SDL_EGL_terminate(SDL_EGL *egl);
void SDL_EGL_free(SDL_EGL *egl);
void SDL_EGL_freep(SDL_EGL **egl);

#endif

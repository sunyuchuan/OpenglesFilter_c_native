/*
* Created by sunyc on 2018-8-9.
* Abstract ijkegl xmegl etc.
*/
#ifndef __APPLE__

#include "sdl_egl.h"

SDL_EGL *SDL_EGL_create(size_t opaque_size)
{
    SDL_EGL *sdl_egl = (SDL_EGL *) calloc(1, sizeof(SDL_EGL));
    if (!sdl_egl)
        return NULL;

    sdl_egl->opaque = calloc(1, opaque_size);
    if (!sdl_egl->opaque) {
        free(sdl_egl);
        return NULL;
    }

    return sdl_egl;
}

bool SDL_EGL_display(SDL_EGL *sdl_egl, EGLNativeWindowType window, SDL_VoutOverlay *overlay)
{
    if (!sdl_egl || !sdl_egl->opaque)
        return false;
    SDL_EGL_Opaque *opaque = sdl_egl->opaque;

    if (sdl_egl->func_display)
        return sdl_egl->func_display(opaque, window, overlay);
    return false;
}

void SDL_EGL_set_bitmap(SDL_EGL *sdl_egl, SDL_XM_Filter_Bitmap *f_bitmap)
{
    if (!sdl_egl || !sdl_egl->opaque)
        return;
    SDL_EGL_Opaque *opaque = sdl_egl->opaque;

    if (sdl_egl->func_set_bitmap)
        sdl_egl->func_set_bitmap(opaque, f_bitmap);
}

void SDL_EGL_terminate(SDL_EGL *sdl_egl)
{
    if (!sdl_egl)
        return;
    SDL_EGL_Opaque *opaque = sdl_egl->opaque;

    if(sdl_egl->func_terminate)
        sdl_egl->func_terminate(opaque);
}

void SDL_EGL_free(SDL_EGL *sdl_egl)
{
    if (!sdl_egl)
        return;
    SDL_EGL_Opaque *opaque = sdl_egl->opaque;

    if (sdl_egl->func_free) {
        sdl_egl->func_free(opaque);
    }

    free(opaque);
    memset(sdl_egl, 0, sizeof(SDL_EGL));
    free(sdl_egl);
}

void SDL_EGL_freep(SDL_EGL **sdl_egl)
{
    if (!sdl_egl || !*sdl_egl)
        return;

    SDL_EGL_free(*sdl_egl);
    *sdl_egl = NULL;
}

#endif

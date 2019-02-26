/*
* Created by sunyc on 2018-7-13.
* off-screen render by opengles
*/

#ifndef XM_SDL_EGL_OFF_SCREEN_RENDERER_H
#define XM_SDL_EGL_OFF_SCREEN_RENDERER_H

#include "ijksdl/sdl_egl.h"

void *XMSDL_EGL_pbuffer_get(SDL_EGL *egl);
SDL_EGL *XMSDL_EGL_OffScreen_create();
#endif

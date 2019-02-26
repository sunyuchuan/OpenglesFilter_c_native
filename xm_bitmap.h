/*
* Created by sunyc on 2018-8-24.
* XM_EGL_BITMAP.
*/

#ifndef XM_BITMAP_H
#define XM_BITMAP_H
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

typedef struct XM_Bitmap
{
    GLubyte *pBitmap;
    int pBmpSize;
    int w;
    int h;
} XM_Bitmap;

#endif

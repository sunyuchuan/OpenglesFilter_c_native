/*
* Created by sunyc on 2018-8-9.
* Abstract ijkegl xmegl etc.
*/

#ifndef __APPLE__

#include "xmsdl_egl_preview_renderer.h"

#include <stdlib.h>
#include <stdbool.h>
#include "ijksdl/xmsdl_gles2.h"
#include "ijksdl/ijksdl_log.h"
#include "ijksdl/ijksdl_vout.h"
#include "ijksdl/gles2_xm/internal.h"
#include "base_filter/base_renderer.h"
#include "ijksdl/xm_bitmap.h"
#ifdef USE_MTCNN_NCNN
#include "ijksdl/xm_mtcnn_ncnn.h"
#endif

typedef struct XM_EGL_Opaque {
    XM_GLES2_Renderer *renderer;
    XM_GLES2_Base_Renderer *b_renderer;
#ifdef USE_MTCNN_NCNN
    XMMtcnnNcnn *mn;
#endif
    XM_Filter_ShaderParameter *shader_param;
    XM_Bitmap xm_bitmap[V_BASE_EFFECTS_TYPE_NB + V_TIME_EFFECTS_TYPE_NB][EGL_BITMAP_NUMBER_MAX];
    int index_bitmap[V_BASE_EFFECTS_TYPE_NB + V_TIME_EFFECTS_TYPE_NB];
    int num_bitmap[V_BASE_EFFECTS_TYPE_NB + V_TIME_EFFECTS_TYPE_NB];
} XM_EGL_Opaque;

typedef struct SDL_EGL_Opaque {
    XM_EGL *xm_egl;
} SDL_EGL_Opaque;

static EGLBoolean XM_EGL_isValid(XM_EGL* egl)
{
    if (egl &&
        egl->window &&
        egl->display &&
        egl->surface &&
        egl->context) {
        return EGL_TRUE;
    }

    return EGL_FALSE;
}

static void XM_EGL_bmp_free(GLubyte *pBitmap)
{
    if (!pBitmap)
        return;

    free(pBitmap);
}

static void XM_EGL_bmp_freep(GLubyte **pBitmap)
{
    if (!pBitmap || !*pBitmap)
        return;

    XM_EGL_bmp_free(*pBitmap);
    *pBitmap = NULL;
}

static void XM_EGL_Bitmap_destory(XM_Bitmap xm_bitmap[][EGL_BITMAP_NUMBER_MAX])
{
    if (!xm_bitmap)
        return;

    for(int i = 0; i < V_BASE_EFFECTS_TYPE_NB + V_TIME_EFFECTS_TYPE_NB; i++)
        for(int j = 0; j < EGL_BITMAP_NUMBER_MAX; j++)
        {
            XM_EGL_bmp_freep(&xm_bitmap[i][j].pBitmap);
        }
}

static void XM_EGL_terminate(XM_EGL *egl)
{
    if (!XM_EGL_isValid(egl))
        return;

    if (egl->opaque)
    {
        XM_GLES2_Renderer_freeP(&egl->opaque->renderer);
        XM_GLES2_BaseRenderer_freeP(&egl->opaque->b_renderer);
        XM_EGL_Bitmap_destory(egl->opaque->xm_bitmap);
#ifdef USE_MTCNN_NCNN
        if(egl->opaque->mn) {
            xm_mtcnn_ncnn_abort(egl->opaque->mn->thread);
            xm_mtcnn_ncnn_wait(egl->opaque->mn->thread);
            xm_mtcnn_ncnn_freep(&egl->opaque->mn);
        }
#endif
        if(egl->opaque->shader_param)
            free(egl->opaque->shader_param);
    }

    if (egl->display) {
        eglMakeCurrent(egl->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (egl->context)
            eglDestroyContext(egl->display, egl->context);
        if (egl->surface)
            eglDestroySurface(egl->display, egl->surface);
        eglTerminate(egl->display);
        eglReleaseThread(); // FIXME: call at thread exit
    }

    egl->context = EGL_NO_CONTEXT;
    egl->surface = EGL_NO_SURFACE;
    egl->display = EGL_NO_DISPLAY;
}

static int XM_EGL_getSurfaceWidth(XM_EGL* egl)
{
    EGLint width = 0;
    if (!eglQuerySurface(egl->display, egl->surface, EGL_WIDTH, &width)) {
        ALOGE("[EGL] eglQuerySurface(EGL_WIDTH) returned error %d", eglGetError());
        return 0;
    }

    return width;
}

static int XM_EGL_getSurfaceHeight(XM_EGL* egl)
{
    EGLint height = 0;
    if (!eglQuerySurface(egl->display, egl->surface, EGL_HEIGHT, &height)) {
        ALOGE("[EGL] eglQuerySurface(EGL_HEIGHT) returned error %d", eglGetError());
        return 0;
    }

    return height;
}

static EGLBoolean XM_EGL_setSurfaceSize(XM_EGL* egl, int width, int height)
{
    if (!XM_EGL_isValid(egl))
        return EGL_FALSE;

#ifdef __ANDROID__
    egl->width  = XM_EGL_getSurfaceWidth(egl);
    egl->height = XM_EGL_getSurfaceHeight(egl);

    if (width != egl->width || height != egl->height) {
        int format = ANativeWindow_getFormat(egl->window);
        ALOGI("ANativeWindow_setBuffersGeometry(w=%d,h=%d) -> (w=%d,h=%d);",
            egl->width, egl->height,
            width, height);
        int ret = ANativeWindow_setBuffersGeometry(egl->window, width, height, format);
        if (ret) {
            ALOGE("[EGL] ANativeWindow_setBuffersGeometry() returned error %d", ret);
            return EGL_FALSE;
        }

        egl->width  = XM_EGL_getSurfaceWidth(egl);
        egl->height = XM_EGL_getSurfaceHeight(egl);
        return (egl->width && egl->height) ? EGL_TRUE : EGL_FALSE;
    }

    return EGL_TRUE;
#else
    // FIXME: other platform?
#endif
    return EGL_FALSE;
}

static EGLBoolean XM_EGL_makeCurrent(XM_EGL* egl, EGLNativeWindowType window)
{
    if (window && window == egl->window &&
        egl->display &&
        egl->surface &&
        egl->context) {

        if (!eglMakeCurrent(egl->display, egl->surface, egl->surface, egl->context)) {
            ALOGE("[EGL] elgMakeCurrent() failed (cached)\n");
            return EGL_FALSE;
        }

        return EGL_TRUE;
    }

    XM_EGL_terminate(egl);
    egl->window = window;

    if (!window)
        return EGL_FALSE;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        ALOGE("[EGL] eglGetDisplay failed\n");
        return EGL_FALSE;
    }

    EGLint major, minor;
    if (!eglInitialize(display, &major, &minor)) {
        ALOGE("[EGL] eglInitialize failed\n");
        return EGL_FALSE;
    }
    ALOGI("[EGL] eglInitialize %d.%d\n", (int)major, (int)minor);

    static const EGLint configAttribs[] = {
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_BLUE_SIZE,          8,
        EGL_GREEN_SIZE,         8,
        EGL_RED_SIZE,           8,
        EGL_NONE
    };

    static const EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfig;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfig)) {
        ALOGE("[EGL] eglChooseConfig failed\n");
        eglTerminate(display);
        return EGL_FALSE;
    }

#ifdef __ANDROID__
    {
        EGLint native_visual_id = 0;
        if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &native_visual_id)) {
            ALOGE("[EGL] eglGetConfigAttrib() returned error %d", eglGetError());
            eglTerminate(display);
            return EGL_FALSE;
        }

        int32_t width  = ANativeWindow_getWidth(window);
        int32_t height = ANativeWindow_getHeight(window);
        ALOGI("[EGL] ANativeWindow_setBuffersGeometry(f=%d);", native_visual_id);
        int ret = ANativeWindow_setBuffersGeometry(window, width, height, native_visual_id);
        if (ret) {
            ALOGE("[EGL] ANativeWindow_setBuffersGeometry(format) returned error %d", ret);
            eglTerminate(display);
            return EGL_FALSE;
        }
    }
#endif

    EGLSurface surface = eglCreateWindowSurface(display, config, window, NULL);
    if (surface == EGL_NO_SURFACE) {
        ALOGE("[EGL] eglCreateWindowSurface failed\n");
        eglTerminate(display);
        return EGL_FALSE;
    }

    EGLSurface context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT) {
        ALOGE("[EGL] eglCreateContext failed\n");
        eglDestroySurface(display, surface);
        eglTerminate(display);
        return EGL_FALSE;
    }

    if (!eglMakeCurrent(display, surface, surface, context)) {
        ALOGE("[EGL] elgMakeCurrent() failed (new)\n");
        eglDestroyContext(display, context);
        eglDestroySurface(display, surface);
        eglTerminate(display);
        return EGL_FALSE;
    }

#if 0
#if defined(__ANDROID__)
    {
        const char *extensions = (const char *) glGetString(GL_EXTENSIONS);
        if (extensions) {
            char *dup_extensions = strdup(extensions);
            if (dup_extensions) {
                char *brk = NULL;
                char *ext = strtok_r(dup_extensions, " ", &brk);
                while (ext) {
                    if (0 == strcmp(ext, "GL_EXT_texture_rg"))
                        egl->gles2_extensions[XM_GLES2__GL_EXT_texture_rg] = 1;

                    ext = strtok_r(NULL, " ", &brk);
                }

                free(dup_extensions);
            }
        }
    }
#elif defined(__APPLE__)
    egl->gles2_extensions[XM_GLES2__GL_EXT_texture_rg] = 1;
#endif

    ALOGI("[EGL] GLES2 extensions begin:\n");
    ALOGI("[EGL]     GL_EXT_texture_rg: %d\n", egl->gles2_extensions[XM_GLES2__GL_EXT_texture_rg]);
    ALOGI("[EGL] GLES2 extensions end.\n");
#endif

    XM_GLES2_Renderer_setupGLES();

    egl->context = context;
    egl->surface = surface;
    egl->display = display;
    return EGL_TRUE;
}

static EGLBoolean XM_EGL_prepareBaseRenderer(XM_EGL* egl, SDL_VoutOverlay *overlay)
{
    assert(egl);
    assert(egl->opaque);
    assert(egl->opaque->renderer);

    XM_EGL_Opaque *opaque = egl->opaque;
    XM_GLES2_Renderer *renderer = opaque->renderer;
    XM_GLES2_Base_Renderer *b_renderer = opaque->b_renderer;
    if(!XM_GLES2_BaseRenderer_isValid(b_renderer, overlay)) {

        if(b_renderer)
        {
            XM_GLES2_BaseRenderer_reset(opaque->b_renderer);
            XM_GLES2_BaseRenderer_freeP(&opaque->b_renderer);
            /*Need to reload Vertices and TexCoords for the others program*/
            if (!XM_GLES2_Renderer_use(opaque->renderer, opaque->shader_param)) {
                XM_GLES2_Renderer_freeP(&opaque->renderer);
                ALOGE("[BEffects prepare] Could not use egl opaque render.");
                return EGL_FALSE;
            }
            renderer->frame_width = 0;//reload XM_GLES2_Renderer_TexCoords_cropRight
            renderer->frame_height = 0;
        }

        /*
        * for XM_GLES2_Renderer_renderOverlay, reload Vertices and TexCoords.
        * or not use  renderer->v_effects.baseType,
        * Reload Vertices and TexCoords before each drawing in *_renderOverlay
        */
        renderer->v_effects.baseType_index = overlay->v_effects.baseType_index;
        renderer->v_effects.baseType_show = overlay->v_effects.baseType_show;
        if(!overlay->v_effects.baseType_show || overlay->v_effects.baseType_index <= V_BASE_EFFECTS_TYPE_NONE) {
            return EGL_TRUE;
        }

        opaque->b_renderer = XM_GLES2_BaseRenderer_create(overlay, opaque->xm_bitmap);
        b_renderer = opaque->b_renderer;
        if (b_renderer == NULL) {
            ALOGE("[BEffects prepare] Could not create BEffects_Renderer.");
            return EGL_FALSE;
        }

        if (!XM_GLES2_BaseRenderer_use(b_renderer, overlay, opaque->shader_param)) {
            ALOGE("[EGL] Could not use BEffects_Program.");
            XM_GLES2_BaseRenderer_freeP(&opaque->b_renderer);
            return EGL_FALSE;
        }
    }

    if(b_renderer && b_renderer->show) {
        glBindFramebuffer(GL_FRAMEBUFFER, b_renderer->fbo);
    }

    return EGL_TRUE;
}

static EGLBoolean XM_EGL_BaseRenderer_use(XM_EGL* egl, SDL_VoutOverlay *overlay)
{
    assert(egl);
    assert(egl->opaque);
    assert(egl->opaque->b_renderer);

    XM_GLES2_Base_Renderer *b_renderer = egl->opaque->b_renderer;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glUseProgram(b_renderer->program);            XM_GLES2_checkError_TRACE("glUseProgram");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!XM_EGL_setSurfaceSize(egl, overlay->w, overlay->h)) {
        ALOGE("[EGL] XM_EGL_setSurfaceSize(%d, %d) failed\n", overlay->w, overlay->h);
        return EGL_FALSE;
    }

    glViewport(0, 0, egl->width, egl->height);  XM_GLES2_checkError_TRACE("glViewport");
    return EGL_TRUE;
}

static EGLBoolean XM_EGL_prepareRenderer(XM_EGL* egl, SDL_VoutOverlay *overlay)
{
    assert(egl);
    assert(egl->opaque);

    XM_EGL_Opaque *opaque = egl->opaque;

    if (!XM_GLES2_Renderer_isValid(opaque->renderer, overlay) ||
        !XM_GLES2_Renderer_isFormat(opaque->renderer, overlay)) {

        XM_GLES2_Renderer_reset(opaque->renderer);
        XM_GLES2_Renderer_freeP(&opaque->renderer);
        /*Need to reload Vertices and TexCoords for the others program*/
        if(opaque->b_renderer) {
            if (!XM_GLES2_BaseRenderer_use(opaque->b_renderer, overlay, opaque->shader_param)) {
                ALOGE("[EGL] Could not use BEffects_Program.");
                XM_GLES2_BaseRenderer_freeP(&opaque->b_renderer);
            }
        }

        opaque->renderer = XM_GLES2_Renderer_create(overlay);
        if (!opaque->renderer) {
            ALOGE("[EGL] Could not create render.");
            return EGL_FALSE;
        }

        XM_Filter_setBitmap(opaque->renderer->time_filter, opaque->xm_bitmap);
        if (!XM_GLES2_Renderer_use(opaque->renderer, opaque->shader_param)) {
            ALOGE("[EGL] Could not use render.");
            XM_GLES2_Renderer_freeP(&opaque->renderer);
            return EGL_FALSE;
        }
    }

    if (!XM_EGL_setSurfaceSize(egl, overlay->w, overlay->h)) {
        ALOGE("[EGL] XM_EGL_setSurfaceSize(%d, %d) failed\n", overlay->w, overlay->h);
        return EGL_FALSE;
    }

    glViewport(0, 0, egl->width, egl->height);  XM_GLES2_checkError_TRACE("glViewport");
    return EGL_TRUE;
}

static EGLBoolean XM_EGL_display_internal(XM_EGL* egl, EGLNativeWindowType window, SDL_VoutOverlay *overlay)
{
    XM_EGL_Opaque *opaque = egl->opaque;

    if (!XM_EGL_prepareRenderer(egl, overlay)) {
        ALOGE("[EGL] XM_EGL_prepareRenderer failed\n");
        return EGL_FALSE;
    }

    if (!XM_EGL_prepareBaseRenderer(egl, overlay)) {
        ALOGE("[EGL] XM_EGL_BEffects_Renderer_prepare failed\n");
        //return EGL_FALSE;
    }

    if (!XM_GLES2_Renderer_renderOverlay(opaque->renderer, overlay, opaque->shader_param)) {
        ALOGE("[EGL] XM_GLES2_render failed\n");
        return EGL_FALSE;
    }

    if(opaque->b_renderer && opaque->b_renderer->show)
    {
        if (!XM_EGL_BaseRenderer_use(egl, overlay)) {
            ALOGE("[EGL] XM_EGL_BEffects_Renderer_use failed\n");
            return EGL_FALSE;
        }

        if (!XM_GLES2_BaseRenderer_renderOverlay(opaque->b_renderer, opaque->renderer->vertices, &opaque->renderer->reload_vertices, opaque->shader_param)) {
            ALOGE("[EGL] XM_GLES2_BEffects_Renderer_renderOverlay failed\n");
            return EGL_FALSE;
        }
     }

    eglSwapBuffers(egl->display, egl->surface);

    return EGL_TRUE;
}

static void XM_EGL_set_bitmap_l(XM_EGL_Opaque *opaque, SDL_XM_Filter_Bitmap *f_bitmap, int index)
{
    if (!opaque || !f_bitmap || !f_bitmap->pixels)
        return;
    XM_Bitmap *xm_bitmap = opaque->xm_bitmap[index];
    int index_bitmap = opaque->index_bitmap[index];

    if(index_bitmap < opaque->num_bitmap[index])
    {
        xm_bitmap[index_bitmap].w = f_bitmap->w;
        xm_bitmap[index_bitmap].h = f_bitmap->h;
        xm_bitmap[index_bitmap].pBmpSize = IJKALIGN(4 * f_bitmap->w * f_bitmap->h, 4);
        xm_bitmap[index_bitmap].pBitmap = (GLubyte *)malloc(xm_bitmap[index_bitmap].pBmpSize);
        memcpy(xm_bitmap[index_bitmap].pBitmap, f_bitmap->pixels, 4 * f_bitmap->w * f_bitmap->h);
        opaque->index_bitmap[index] ++;
        ALOGI("preview XM_EGL_set_bitmap_l index %d, index_bitmap %d", index, opaque->index_bitmap[index]);
    }
}

static void XM_EGL_set_bitmap(XM_EGL *egl, SDL_XM_Filter_Bitmap *f_bitmap)
{
    if (!egl || !f_bitmap)
        return;

    XM_EGL_Opaque *opaque = egl->opaque;
    switch(f_bitmap->type) {
        case V_BASE_EFFECTS_TYPE:
            XM_EGL_set_bitmap_l(opaque, f_bitmap, f_bitmap->filter_index);
            break;
        case V_TIME_EFFECTS_TYPE:
            XM_EGL_set_bitmap_l(opaque, f_bitmap, V_BASE_EFFECTS_TYPE_NB + f_bitmap->filter_index);
            break;
        default :
            break;
    }
}

#ifdef USE_MTCNN_NCNN
static EGLBoolean XM_EGL_Face_Detect(XM_EGL_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    XMMtcnnNcnn *mn = opaque->mn;
    XM_Filter_ShaderParameter *param = opaque->shader_param;
    if(!mn || !param)
        return false;

    if(!xm_mtcnn_ncnn_faceDetect(mn, overlay, param->face_rectangle_coordinates))
    {
        //ALOGE("xm_mtcnn_ncnn_faceDetect failed or not activated\n");
    }
    return true;
}
#endif

static EGLBoolean XM_EGL_display(XM_EGL *egl, EGLNativeWindowType window, SDL_VoutOverlay *overlay)
{
    EGLBoolean ret = EGL_FALSE;
    if (!egl)
        return EGL_FALSE;

    XM_EGL_Opaque *opaque = egl->opaque;
    if (!opaque)
        return EGL_FALSE;

#ifdef USE_MTCNN_NCNN
    XM_EGL_Face_Detect(opaque, overlay);
#endif

    if (!XM_EGL_makeCurrent(egl, window))
        return EGL_FALSE;

    ret = XM_EGL_display_internal(egl, window, overlay);
    eglMakeCurrent(egl->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglReleaseThread(); // FIXME: call at thread exit
    return ret;
}

static void XM_EGL_releaseWindow(XM_EGL* egl)
{
    if (!egl)
        return;

    XM_EGL_terminate(egl);
}

static void XM_EGL_free(XM_EGL *egl)
{
    if (!egl)
        return;

    XM_EGL_terminate(egl);

    free(egl->opaque);
    memset(egl, 0, sizeof(XM_EGL));
    free(egl);
}

static void XM_EGL_freep(XM_EGL **egl)
{
    if (!egl || !*egl)
        return;

    XM_EGL_free(*egl);
    *egl = NULL;
}

static int XM_EGL_getBitmapNumber(int index)
{
    if(index < V_BASE_EFFECTS_TYPE_NB) {
        return XM_BFilter_getBitmapNumber(index);
    } else {
        return XM_TFilter_getBitmapNumber(index - V_BASE_EFFECTS_TYPE_NB);
    }
}

static SDL_Class g_class = {
    .name = "XMSDL_EGL_preview_renderer",
};

static XM_EGL *XM_EGL_create()
{
    XM_EGL *egl = (XM_EGL *) mallocz(sizeof(XM_EGL));
    if (!egl)
        return NULL;
    memset(egl, 0, sizeof(XM_EGL));

    egl->opaque_class = &g_class;
    egl->opaque = mallocz(sizeof(XM_EGL_Opaque));
    if (!egl->opaque) {
        free(egl);
        return NULL;
    }
    memset(egl->opaque, 0, sizeof(XM_EGL_Opaque));
    for(int i = 0; i < V_BASE_EFFECTS_TYPE_NB + V_TIME_EFFECTS_TYPE_NB; i++)
        egl->opaque->num_bitmap[i] = XM_EGL_getBitmapNumber(i);

#ifdef USE_MTCNN_NCNN
    egl->opaque->mn = xm_mtcnn_ncnn_create();
#endif
    egl->opaque->shader_param = (XM_Filter_ShaderParameter *)mallocz(sizeof(XM_Filter_ShaderParameter));
    memset(egl->opaque->shader_param, 0, sizeof(XM_Filter_ShaderParameter));
    egl->opaque->shader_param->off_screen = false;

    return egl;
}

static void XMSDL_EGL_free(SDL_EGL_Opaque *opaque)
{
    if (!opaque || !opaque->xm_egl)
        return;

    XM_EGL_freep(&opaque->xm_egl);
}

static void XMSDL_EGL_terminate(SDL_EGL_Opaque *opaque)
{
    if (!opaque || !opaque->xm_egl)
        return;

    XM_EGL_terminate(opaque->xm_egl);
}

static void XMSDL_EGL_set_bitmap(SDL_EGL_Opaque *opaque, SDL_XM_Filter_Bitmap *f_bitmap)
{
    if (!opaque || !opaque->xm_egl)
        return;

    return XM_EGL_set_bitmap(opaque->xm_egl, f_bitmap);
}

static EGLBoolean XMSDL_EGL_display(SDL_EGL_Opaque *opaque, EGLNativeWindowType window, SDL_VoutOverlay *overlay)
{
    if (!opaque || !opaque->xm_egl)
        return EGL_FALSE;

    return XM_EGL_display(opaque->xm_egl, window, overlay);
}

SDL_EGL *XMSDL_EGL_preview_create()
{
    SDL_EGL *sdl_egl = SDL_EGL_create(sizeof(SDL_EGL_Opaque));
    if (!sdl_egl)
        return NULL;

    SDL_EGL_Opaque *opaque = sdl_egl->opaque;
    opaque->xm_egl = XM_EGL_create();

    sdl_egl->func_display  = XMSDL_EGL_display;
    sdl_egl->func_set_bitmap = XMSDL_EGL_set_bitmap;
    sdl_egl->func_terminate = XMSDL_EGL_terminate;
    sdl_egl->func_free = XMSDL_EGL_free;

    return sdl_egl;
}
#endif

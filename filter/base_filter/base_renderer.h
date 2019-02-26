/*
* Created by sunyc on 2018-8-14.
* Abstraction and packaging about gles effects.
*/
#ifndef XM_BASE_RENDERER_EFFECTS_H
#define XM_BASE_RENDERER_EFFECTS_H

#include <stdbool.h>
#include "ijksdl/filter/xm_filter.h"
#include "ijksdl/filter/xm_filter_factory.h"
#include "ijksdl/xmsdl_gles2.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"
#include "ijksdl/xm_bitmap.h"

typedef struct XM_GLES2_Base_Renderer XM_GLES2_Base_Renderer;
typedef struct SDL_VoutOverlay SDL_VoutOverlay;

typedef struct XM_GLES2_Base_Renderer
{
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint av4_position;
    GLuint av2_texcoord;
    GLuint um4_mvp;

    GLuint fbo;
    GLuint us2_sampler;
    GLuint textures[1];
    GLuint renderBuffer;
    //GLuint texture_num;
    XM_Filter *base_filter;
    GLboolean (*func_use)(XM_GLES2_Base_Renderer *b_renderer, SDL_VoutOverlay *overlay);
    GLboolean (*func_uploadTexture)(XM_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay);

    GLfloat texcoords[8];
    GLfloat vertices[8];
    GLfloat aspect_ratio;
    GLfloat w;
    GLfloat h;
    int index;
    bool show;
} XM_GLES2_Base_Renderer;

GLboolean XM_GLES2_BaseRenderer_isValid(XM_GLES2_Base_Renderer *b_renderer, SDL_VoutOverlay *overlay);
void XM_GLES2_BaseRenderer_free(XM_GLES2_Base_Renderer *b_renderer);
void XM_GLES2_BaseRenderer_freeP(XM_GLES2_Base_Renderer **b_renderer);
void XM_GLES2_BaseRenderer_reset(XM_GLES2_Base_Renderer *b_renderer);
GLboolean XM_GLES2_BaseRenderer_renderOverlay(XM_GLES2_Base_Renderer *b_renderer, GLfloat vertices[8], bool *reload_vertices, XM_Filter_ShaderParameter *shader_param);
GLboolean XM_GLES2_BaseRenderer_use(XM_GLES2_Base_Renderer *b_renderer, SDL_VoutOverlay *overlay, XM_Filter_ShaderParameter *shader_param);
GLboolean XM_GLES2_BaseRenderer_uploadTexture(XM_GLES2_Base_Renderer *b_renderer, SDL_VoutOverlay *overlay);
XM_GLES2_Base_Renderer *XM_GLES2_BaseRenderer_create(SDL_VoutOverlay *overlay, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX]);

#endif

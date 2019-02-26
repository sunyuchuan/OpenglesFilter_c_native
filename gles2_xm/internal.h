/*
 * copyright (c) 2016 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef XMSDL__IJKSDL_GLES2__INTERNAL__H
#define XMSDL__IJKSDL_GLES2__INTERNAL__H

#include <assert.h>
#include <stdlib.h>
#include "ijksdl/ijksdl_fourcc.h"
#include "ijksdl/ijksdl_log.h"
#include "ijksdl/xmsdl_gles2.h"
#include "ijksdl/ijksdl_vout.h"
#include "ijksdl/filter/xm_filter.h"
#include "ijksdl/filter/xm_filter_factory.h"
#include "ijksdl/filter/xm_filter_texture_rotate.h"

#define XM_GLES_STRINGIZE(x)   #x
#define XM_GLES_STRINGIZE2(x)  XM_GLES_STRINGIZE(x)
#define XM_GLES_STRING(x)      XM_GLES_STRINGIZE2(x)

typedef struct XM_GLES2_Renderer_Opaque XM_GLES2_Renderer_Opaque;

typedef struct XM_GLES2_Renderer
{
    XM_GLES2_Renderer_Opaque *opaque;

    GLuint program;

    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint plane_textures[XM_GLES2_MAX_PLANE];
    //GLuint pbo[2];

    GLuint av4_position;
    GLuint av2_texcoord;
    GLuint um4_mvp;

    GLuint us2_sampler[XM_GLES2_MAX_PLANE];
    GLuint um3_color_conversion;

    GLboolean (*func_use)(XM_GLES2_Renderer *renderer);
    GLsizei   (*func_getBufferWidth)(XM_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay);
    GLboolean (*func_uploadTexture)(XM_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay);
    GLvoid    (*func_destroy)(XM_GLES2_Renderer *renderer);

    GLsizei buffer_width;
    GLsizei visible_width;

    GLfloat texcoords[8];

    GLfloat vertices[8];
    int     vertices_changed;

    int     format;
    int     gravity;
    GLsizei layer_width;
    GLsizei layer_height;
    int     frame_width;
    int     frame_height;
    int     frame_sar_num;
    int     frame_sar_den;
    bool reload_vertices;
    GLfloat cropRight;
    GLfloat aspect_ratio;
    SDL_VideoEffects v_effects;
    XM_Filter *time_filter;

    GLsizei last_buffer_width;
} XM_GLES2_Renderer;

typedef struct XM_GLES_Matrix
{
    GLfloat m[16];
} XM_GLES_Matrix;
void XM_GLES2_loadOrtho(XM_GLES_Matrix *matrix, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);
void XM_GLES2_loadRotateMatrix(XM_GLES_Matrix *matrix, GLfloat angle, GLfloat aspect, GLfloat scale);

const GLfloat *XM_GLES2_getColorMatrix_bt709();
const GLfloat *XM_GLES2_getColorMatrix_bt601();

XM_GLES2_Renderer *XM_GLES2_Renderer_create_base(const char *fragment_shader_source, XM_Filter *filter);
XM_GLES2_Renderer *XM_GLES2_Renderer_create_yuv420p(SDL_VoutOverlay *overlay);
XM_GLES2_Renderer *XM_GLES2_Renderer_create_yuv444p10le(SDL_VoutOverlay *overlay);
XM_GLES2_Renderer *XM_GLES2_Renderer_create_yuv420sp(SDL_VoutOverlay *overlay);
XM_GLES2_Renderer *XM_GLES2_Renderer_create_yuv420sp_vtb(SDL_VoutOverlay *overlay);
XM_GLES2_Renderer *XM_GLES2_Renderer_create_rgb565(SDL_VoutOverlay *overlay);
XM_GLES2_Renderer *XM_GLES2_Renderer_create_rgb888(SDL_VoutOverlay *overlay);
XM_GLES2_Renderer *XM_GLES2_Renderer_create_rgbx8888(SDL_VoutOverlay *overlay);

#endif

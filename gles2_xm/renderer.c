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

#include "gles2_xm/internal.h"
#include "time_filter/time_filter.h"

static void XM_GLES2_printProgramInfo(GLuint program)
{
    if (!program)
        return;

    GLint info_len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
    if (!info_len) {
        ALOGE("[GLES2][Program] empty info\n");
        return;
    }

    char    buf_stack[32];
    char   *buf_heap = NULL;
    char   *buf      = buf_stack;
    GLsizei buf_len  = sizeof(buf_stack) - 1;
    if (info_len > sizeof(buf_stack)) {
        buf_heap = (char*) malloc(info_len + 1);
        if (buf_heap) {
            buf     = buf_heap;
            buf_len = info_len;
        }
    }

    glGetProgramInfoLog(program, buf_len, NULL, buf);
    ALOGE("[GLES2][Program] error %s\n", buf);

    if (buf_heap)
        free(buf_heap);
}

void XM_GLES2_Renderer_reset(XM_GLES2_Renderer *renderer)
{
    if (!renderer)
        return;

    if (renderer->vertex_shader)
        glDeleteShader(renderer->vertex_shader);
    if (renderer->fragment_shader)
        glDeleteShader(renderer->fragment_shader);
    if (renderer->program)
        glDeleteProgram(renderer->program);

    renderer->vertex_shader   = 0;
    renderer->fragment_shader = 0;
    renderer->program         = 0;

    for (int i = 0; i < XM_GLES2_MAX_PLANE; ++i) {
        if (renderer->plane_textures[i]) {
            glDeleteTextures(1, &renderer->plane_textures[i]);
            renderer->plane_textures[i] = 0;
        }
    }

    XM_Filter_reset(renderer->time_filter);

    /*for (int i = 0; i < 2; ++i) {
        if (renderer->pbo[i]) {
            glDeleteBuffer(1, &renderer->pbo[i]);
            renderer->pbo[i] = 0;
        }
    }*/
}

void XM_GLES2_Renderer_free(XM_GLES2_Renderer *renderer)
{
    if (!renderer)
        return;

    XM_Filter_freep(&renderer->time_filter);
    if (renderer->func_destroy)
        renderer->func_destroy(renderer);

#if 0
    if (renderer->vertex_shader)    ALOGW("[GLES2] renderer: vertex_shader not deleted.\n");
    if (renderer->fragment_shader)  ALOGW("[GLES2] renderer: fragment_shader not deleted.\n");
    if (renderer->program)          ALOGW("[GLES2] renderer: program not deleted.\n");

    for (int i = 0; i < XM_GLES2_MAX_PLANE; ++i) {
        if (renderer->plane_textures[i])
            ALOGW("[GLES2] renderer: plane texture[%d] not deleted.\n", i);
    }
#endif

    free(renderer);
}

void XM_GLES2_Renderer_freeP(XM_GLES2_Renderer **renderer)
{
    if (!renderer || !*renderer)
        return;

    XM_GLES2_Renderer_free(*renderer);
    *renderer = NULL;
}

XM_GLES2_Renderer *XM_GLES2_Renderer_create_base(const char *fsh, XM_Filter *filter)
{
    assert(fsh);

    XM_GLES2_Renderer *renderer = (XM_GLES2_Renderer *)calloc(1, sizeof(XM_GLES2_Renderer));
    if (!renderer)
        goto fail;
    renderer->cropRight = 0.0f;

    const char *vsh = XM_GLES2_getVertexShader_default();
    if(filter)
    {
        renderer->v_effects.timeType_index= filter->v_effects.timeType_index;
        renderer->v_effects.timeType_show= filter->v_effects.timeType_show;
        renderer->time_filter = filter;
        vsh = XM_Filter_getVertexShader(filter);
        XM_Filter_setRenderer(filter, renderer);
    }

    renderer->vertex_shader = XM_GLES2_loadShader(GL_VERTEX_SHADER, vsh);
    if (!renderer->vertex_shader)
        goto fail;

    renderer->fragment_shader = XM_GLES2_loadShader(GL_FRAGMENT_SHADER, fsh);
    if (!renderer->fragment_shader)
        goto fail;

    renderer->program = glCreateProgram();                          XM_GLES2_checkError("glCreateProgram");
    if (!renderer->program)
        goto fail;

    glAttachShader(renderer->program, renderer->vertex_shader);     XM_GLES2_checkError("glAttachShader(vertex)");
    glAttachShader(renderer->program, renderer->fragment_shader);   XM_GLES2_checkError("glAttachShader(fragment)");
    glLinkProgram(renderer->program);                               XM_GLES2_checkError("glLinkProgram");
    GLint link_status = GL_FALSE;
    glGetProgramiv(renderer->program, GL_LINK_STATUS, &link_status);
    if (!link_status)
        goto fail;

    renderer->av4_position = glGetAttribLocation(renderer->program, "av4_Position");                XM_GLES2_checkError_TRACE("glGetAttribLocation(av4_Position)");
    renderer->av2_texcoord = glGetAttribLocation(renderer->program, "av2_Texcoord");                XM_GLES2_checkError_TRACE("glGetAttribLocation(av2_Texcoord)");
    renderer->um4_mvp      = glGetUniformLocation(renderer->program, "um4_ModelViewProjection");    XM_GLES2_checkError_TRACE("glGetUniformLocation(um4_ModelViewProjection)");

    XM_Filter_VertexShader_init(filter);
    return renderer;
fail:
    if (renderer->program)
        XM_GLES2_printProgramInfo(renderer->program);

    XM_GLES2_Renderer_free(renderer);
    return NULL;
}

XM_GLES2_Renderer *XM_GLES2_Renderer_create(SDL_VoutOverlay *overlay)
{
    if (!overlay)
        return NULL;

    XM_GLES2_printString("Version", GL_VERSION);
    XM_GLES2_printString("Vendor", GL_VENDOR);
    XM_GLES2_printString("Renderer", GL_RENDERER);
    XM_GLES2_printString("Extensions", GL_EXTENSIONS);

    XM_GLES2_Renderer *renderer = NULL;
    switch (overlay->format) {
        case SDL_FCC_RV16:      renderer = XM_GLES2_Renderer_create_rgb565(overlay); break;
        case SDL_FCC_RV24:      renderer = XM_GLES2_Renderer_create_rgb888(overlay); break;
        case SDL_FCC_RV32:      renderer = XM_GLES2_Renderer_create_rgbx8888(overlay); break;
#ifdef __APPLE__
        case SDL_FCC_NV12:      renderer = XM_GLES2_Renderer_create_yuv420sp(overlay); break;
        case SDL_FCC__VTB:      renderer = XM_GLES2_Renderer_create_yuv420sp_vtb(overlay); break;
#endif
        case SDL_FCC_YV12:      renderer = XM_GLES2_Renderer_create_yuv420p(overlay); break;
        case SDL_FCC_I420:      renderer = XM_GLES2_Renderer_create_yuv420p(overlay); break;
        case SDL_FCC_I444P10LE: renderer = XM_GLES2_Renderer_create_yuv444p10le(overlay); break;
        default:
            ALOGE("[GLES2] unknown format %4s(%d)\n", (char *)&overlay->format, overlay->format);
            return NULL;
    }

    renderer->format = overlay->format;
    renderer->v_effects.baseType_index = overlay->v_effects.baseType_index;
    renderer->v_effects.baseType_show = overlay->v_effects.baseType_show;
    renderer->aspect_ratio = (double)overlay->w / (double)overlay->h;
    return renderer;
}

GLboolean XM_GLES2_Renderer_isValid(XM_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay)
{
    return renderer
		&& XM_GLES2_TimeFilter_isValid(renderer, overlay)
		&& renderer->program ? GL_TRUE : GL_FALSE;
}

GLboolean XM_GLES2_Renderer_isFormat(XM_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay)
{
    if (!XM_GLES2_Renderer_isValid(renderer, overlay))
        return GL_FALSE;

    return renderer->format == overlay->format ? GL_TRUE : GL_FALSE;
}

/*
 * Per-Context routine
 */
GLboolean XM_GLES2_Renderer_setupGLES()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);       XM_GLES2_checkError_TRACE("glClearColor");
    glEnable(GL_CULL_FACE);                     XM_GLES2_checkError_TRACE("glEnable(GL_CULL_FACE)");
    glCullFace(GL_BACK);                        XM_GLES2_checkError_TRACE("glCullFace");
    glDisable(GL_DEPTH_TEST);

    return GL_TRUE;
}

static void XM_GLES2_Renderer_Vertices_reset(XM_GLES2_Renderer *renderer)
{
    renderer->vertices[0] = -1.0f;
    renderer->vertices[1] = -1.0f;
    renderer->vertices[2] =  1.0f;
    renderer->vertices[3] = -1.0f;
    renderer->vertices[4] = -1.0f;
    renderer->vertices[5] =  1.0f;
    renderer->vertices[6] =  1.0f;
    renderer->vertices[7] =  1.0f;
}

static void XM_GLES2_Renderer_Vertices_apply(XM_GLES2_Renderer *renderer)
{
    switch (renderer->gravity) {
        case XM_GLES2_GRAVITY_RESIZE_ASPECT:
            break;
        case XM_GLES2_GRAVITY_RESIZE_ASPECT_FILL:
            break;
        case XM_GLES2_GRAVITY_RESIZE:
            XM_GLES2_Renderer_Vertices_reset(renderer);
            return;
        default:
            ALOGE("[GLES2] unknown gravity %d\n", renderer->gravity);
            XM_GLES2_Renderer_Vertices_reset(renderer);
            return;
    }

    if (renderer->layer_width <= 0 ||
        renderer->layer_height <= 0 ||
        renderer->frame_width <= 0 ||
        renderer->frame_height <= 0)
    {
        ALOGE("[GLES2] invalid width/height for gravity aspect\n");
        XM_GLES2_Renderer_Vertices_reset(renderer);
        return;
    }

    float width     = renderer->frame_width;
    float height    = renderer->frame_height;

    if (renderer->frame_sar_num > 0 && renderer->frame_sar_den > 0) {
        width = width * renderer->frame_sar_num / renderer->frame_sar_den;
    }

    const float dW  = (float)renderer->layer_width	/ width;
    const float dH  = (float)renderer->layer_height / height;
    float dd        = 1.0f;
    float nW        = 1.0f;
    float nH        = 1.0f;

    switch (renderer->gravity) {
        case XM_GLES2_GRAVITY_RESIZE_ASPECT_FILL:  dd = FFMAX(dW, dH); break;
        case XM_GLES2_GRAVITY_RESIZE_ASPECT:       dd = FFMIN(dW, dH); break;
    }

    nW = (width  * dd / (float)renderer->layer_width);
    nH = (height * dd / (float)renderer->layer_height);

    renderer->vertices[0] = - nW;
    renderer->vertices[1] = - nH;
    renderer->vertices[2] =   nW;
    renderer->vertices[3] = - nH;
    renderer->vertices[4] = - nW;
    renderer->vertices[5] =   nH;
    renderer->vertices[6] =   nW;
    renderer->vertices[7] =   nH;
}

static void XM_GLES2_Renderer_Vertices_reloadVertex(XM_GLES2_Renderer *renderer)
{
    glVertexAttribPointer(renderer->av4_position, 2, GL_FLOAT, GL_FALSE, 0, renderer->vertices);    XM_GLES2_checkError_TRACE("glVertexAttribPointer(av2_texcoord)");
    glEnableVertexAttribArray(renderer->av4_position);                                      XM_GLES2_checkError_TRACE("glEnableVertexAttribArray(av2_texcoord)");
}

#define XM_GLES2_GRAVITY_MIN                   (0)
#define XM_GLES2_GRAVITY_RESIZE                (0) // Stretch to fill layer bounds.
#define XM_GLES2_GRAVITY_RESIZE_ASPECT         (1) // Preserve aspect ratio; fit within layer bounds.
#define XM_GLES2_GRAVITY_RESIZE_ASPECT_FILL    (2) // Preserve aspect ratio; fill layer bounds.
#define XM_GLES2_GRAVITY_MAX                   (2)

GLboolean XM_GLES2_Renderer_setGravity(XM_GLES2_Renderer *renderer, int gravity, GLsizei layer_width, GLsizei layer_height)
{
    if (renderer->gravity != gravity && gravity >= XM_GLES2_GRAVITY_MIN && gravity <= XM_GLES2_GRAVITY_MAX)
        renderer->vertices_changed = 1;
    else if (renderer->layer_width != layer_width)
        renderer->vertices_changed = 1;
    else if (renderer->layer_height != layer_height)
        renderer->vertices_changed = 1;
    else
        return GL_TRUE;

    renderer->gravity      = gravity;
    renderer->layer_width  = layer_width;
    renderer->layer_height = layer_height;
    return GL_TRUE;
}

static void XM_GLES2_Renderer_TexCoords_reset(XM_GLES2_Renderer *renderer)
{
    renderer->texcoords[0] = 0.0f;
    renderer->texcoords[1] = 1.0f;
    renderer->texcoords[2] = 1.0f;
    renderer->texcoords[3] = 1.0f;
    renderer->texcoords[4] = 0.0f;
    renderer->texcoords[5] = 0.0f;
    renderer->texcoords[6] = 1.0f;
    renderer->texcoords[7] = 0.0f;
}

static void XM_GLES2_Renderer_TexCoords_cropRight(XM_GLES2_Renderer *renderer, GLfloat cropRight)
{
    renderer->texcoords[0] = 0.0f;
    renderer->texcoords[1] = 1.0f;
    renderer->texcoords[2] = 1.0f - cropRight;
    renderer->texcoords[3] = 1.0f;
    renderer->texcoords[4] = 0.0f;
    renderer->texcoords[5] = 0.0f;
    renderer->texcoords[6] = 1.0f - cropRight;
    renderer->texcoords[7] = 0.0f;
}

static void XM_GLES2_Renderer_TexCoords_reloadVertex(XM_GLES2_Renderer *renderer)
{
    glVertexAttribPointer(renderer->av2_texcoord, 2, GL_FLOAT, GL_FALSE, 0, renderer->texcoords);   XM_GLES2_checkError_TRACE("glVertexAttribPointer(av2_texcoord)");
    glEnableVertexAttribArray(renderer->av2_texcoord);                                              XM_GLES2_checkError_TRACE("glEnableVertexAttribArray(av2_texcoord)");
}

/*
 * Per-Renderer routine
 */
GLboolean XM_GLES2_Renderer_use(XM_GLES2_Renderer *renderer, XM_Filter_ShaderParameter *param)
{
    if (!renderer)
        return GL_FALSE;

    assert(renderer->func_use);
    if (!renderer->func_use(renderer))
        return GL_FALSE;

    XM_GLES_Matrix modelViewProj;
    XM_GLES2_loadOrtho(&modelViewProj, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(renderer->um4_mvp, 1, GL_FALSE, modelViewProj.m);                    XM_GLES2_checkError_TRACE("glUniformMatrix4fv(um4_mvp)");

    XM_GLES2_Renderer_TexCoords_reset(renderer);
    if(param->off_screen)
    {
        XM_Filter_TexCoords_Rotation(renderer->texcoords, ROTATION_180, false, false);
    }
    XM_Filter_ShaderVariables_reset(renderer->time_filter, param);
    XM_GLES2_Renderer_TexCoords_reloadVertex(renderer);

    XM_GLES2_Renderer_Vertices_reset(renderer);
    XM_GLES2_Renderer_Vertices_reloadVertex(renderer);

    return GL_TRUE;
}

/*
 * Per-Frame routine
 */
GLboolean XM_GLES2_Renderer_renderOverlay(XM_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay, XM_Filter_ShaderParameter *param)
{
    if (!renderer || !renderer->func_uploadTexture)
        return GL_FALSE;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glUseProgram(renderer->program);            XM_GLES2_checkError_TRACE("glUseProgram");
    glClear(GL_COLOR_BUFFER_BIT);               XM_GLES2_checkError_TRACE("glClear");

    GLsizei visible_width  = renderer->frame_width;
    GLsizei visible_height = renderer->frame_height;
    if (overlay) {
        visible_width  = overlay->w;
        visible_height = overlay->h;
        if (renderer->frame_width   != visible_width    ||
            renderer->frame_height  != visible_height   ||
            renderer->frame_sar_num != overlay->sar_num ||
            renderer->frame_sar_den != overlay->sar_den) {

            renderer->frame_width   = visible_width;
            renderer->frame_height  = visible_height;
            renderer->frame_sar_num = overlay->sar_num;
            renderer->frame_sar_den = overlay->sar_den;

            renderer->vertices_changed = 1;
        }

        renderer->last_buffer_width = renderer->func_getBufferWidth(renderer, overlay);

        if (!renderer->func_uploadTexture(renderer, overlay))
            return GL_FALSE;
    } else {
        // NULL overlay means force reload vertice
        renderer->vertices_changed = 1;
    }

    GLsizei buffer_width = renderer->last_buffer_width;
    if (renderer->vertices_changed ||
        (buffer_width > 0 &&
         buffer_width > visible_width &&
         buffer_width != renderer->buffer_width &&
         visible_width != renderer->visible_width)){

        renderer->vertices_changed = 0;

        XM_GLES2_Renderer_Vertices_apply(renderer);
        XM_GLES2_Renderer_Vertices_reloadVertex(renderer);

        renderer->buffer_width  = buffer_width;
        renderer->visible_width = visible_width;
        renderer->reload_vertices = true;

        GLsizei padding_pixels     = buffer_width - visible_width;
        GLfloat padding_normalized = ((GLfloat)padding_pixels) / buffer_width;
        renderer->cropRight = padding_normalized;

        XM_GLES2_Renderer_TexCoords_reset(renderer);
        XM_GLES2_Renderer_TexCoords_cropRight(renderer, padding_normalized);
        if(param->off_screen)
        {
            XM_Filter_TexCoords_Rotation(renderer->texcoords, ROTATION_180, false, false);
        }
        XM_Filter_ShaderVariables_reset(renderer->time_filter, param);
        XM_GLES2_Renderer_TexCoords_reloadVertex(renderer);
    }

    if(renderer->v_effects.baseType_show)
    {
        XM_GLES2_Renderer_Vertices_reloadVertex(renderer);
        XM_GLES2_Renderer_TexCoords_reloadVertex(renderer);
    }
    XM_Filter_setShaderVariables(renderer->time_filter, param);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);      XM_GLES2_checkError_TRACE("glDrawArrays");

    return GL_TRUE;
}

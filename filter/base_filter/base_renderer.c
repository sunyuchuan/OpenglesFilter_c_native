/*
* Created by sunyc on 2018-8-14.
* Abstraction and packaging about gles effects.
*/

#include "base_renderer.h"
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

static GLboolean use(XM_GLES2_Base_Renderer *b_renderer, SDL_VoutOverlay *overlay)
{
    GLboolean ret = GL_FALSE;
    if (!b_renderer || !overlay)
        return ret ;

    int textures_num = XM_TFilter_yuv420p_getPlanesTexturesNum(&overlay->v_effects);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glUseProgram(b_renderer->program);            XM_GLES2_checkError_TRACE("glUseProgram");

    if (0 == b_renderer->textures[0])
        glGenTextures(1, b_renderer->textures);

    glActiveTexture(GL_TEXTURE0 + textures_num);
    glBindTexture(GL_TEXTURE_2D, b_renderer->textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, overlay->w, overlay->h,
                        0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glUniform1i(b_renderer->us2_sampler, textures_num);

    if(0 == b_renderer->fbo) {
        glGenFramebuffers(1, &b_renderer->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, b_renderer->fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, b_renderer->textures[0], 0);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE)
        {
            ALOGE("Problem with OpenGL framebuffer after specifying color render buffer: %x\n", status);
        } else {
            ALOGI("FBO creation succedded\n");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ret = XM_Filter_yuv420p_use(b_renderer->base_filter, textures_num + 1);
    return ret;
}

static GLboolean uploadTexture(XM_GLES2_Base_Renderer *b_renderer, SDL_VoutOverlay *overlay)
{
    if (!b_renderer)
        return GL_FALSE;

    return XM_Filter_yuv420p_uploadTexture(b_renderer->base_filter, overlay);
}

GLboolean XM_GLES2_BaseRenderer_isValid(XM_GLES2_Base_Renderer *b_renderer, SDL_VoutOverlay *overlay)
{
    return b_renderer
        && (b_renderer->show == overlay->v_effects.baseType_show)
        && (b_renderer->index == overlay->v_effects.baseType_index)
        && b_renderer->program ? GL_TRUE : GL_FALSE;
}

void XM_GLES2_BaseRenderer_free(XM_GLES2_Base_Renderer *b_renderer)
{
    if (!b_renderer)
        return;

    XM_Filter_freep(&b_renderer->base_filter);
    free(b_renderer);
}

void XM_GLES2_BaseRenderer_freeP(XM_GLES2_Base_Renderer **b_renderer)
{
    if (!b_renderer || !*b_renderer)
        return;

    XM_GLES2_BaseRenderer_free(*b_renderer);
    *b_renderer = NULL;
}

void XM_GLES2_BaseRenderer_reset(XM_GLES2_Base_Renderer *b_renderer)
{
    if (!b_renderer)
        return;

    if (b_renderer->vertex_shader)
        glDeleteShader(b_renderer->vertex_shader);
    if (b_renderer->fragment_shader)
        glDeleteShader(b_renderer->fragment_shader);
    if (b_renderer->program)
        glDeleteProgram(b_renderer->program);

    b_renderer->vertex_shader   = 0;
    b_renderer->fragment_shader = 0;
    b_renderer->program         = 0;

    for (int i = 0; i < 1; ++i) {
        if (b_renderer->textures[i]) {
            glDeleteTextures(1, &b_renderer->textures[i]);
            b_renderer->textures[i] = 0;
        }
    }

    if (b_renderer->renderBuffer) {
        glDeleteRenderbuffers(1, &b_renderer->renderBuffer);
        b_renderer->renderBuffer = 0;
    }

    if (b_renderer->fbo) {
        glDeleteFramebuffers(1, &b_renderer->fbo);
        b_renderer->fbo = 0;
    }

    XM_Filter_reset(b_renderer->base_filter);
    //memset(b_renderer, 0, sizeof(XM_GLES2_Base_Renderer));
}

static void XM_GLES2_BaseRenderer_reloadShaderParameter(XM_GLES2_Base_Renderer *b_renderer)
{
    if (!b_renderer)
        return;

    XM_GLES_Matrix modelViewProj;
    XM_GLES2_loadOrtho(&modelViewProj, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(b_renderer->um4_mvp, 1, GL_FALSE, modelViewProj.m);                    XM_GLES2_checkError_TRACE("glUniformMatrix4fv(um4_mvp)");

    glVertexAttribPointer(b_renderer->av4_position, 2, GL_FLOAT, GL_FALSE, 0, b_renderer->vertices);	XM_GLES2_checkError_TRACE("glVertexAttribPointer(av4_position)");
    glEnableVertexAttribArray(b_renderer->av4_position);										XM_GLES2_checkError_TRACE("glEnableVertexAttribArray(av4_position)");

    glVertexAttribPointer(b_renderer->av2_texcoord, 2, GL_FLOAT, GL_FALSE, 0, b_renderer->texcoords);   XM_GLES2_checkError_TRACE("glVertexAttribPointer(av2_texcoord)");
    glEnableVertexAttribArray(b_renderer->av2_texcoord);
}

GLboolean XM_GLES2_BaseRenderer_renderOverlay(XM_GLES2_Base_Renderer *b_renderer, GLfloat vertices[8], bool *reload_vertices, XM_Filter_ShaderParameter *shader_param)
{
    if (!b_renderer || !vertices || !reload_vertices)
        return GL_FALSE;

    glClear(GL_COLOR_BUFFER_BIT);               XM_GLES2_checkError_TRACE("glClear");
    //glActiveTexture(GL_TEXTURE0 + b_renderer->texture_num);
    glBindTexture(GL_TEXTURE_2D, b_renderer->textures[0]);

    if (*reload_vertices) {
        *reload_vertices = false;
        for(int i =0 ; i < 8; i++)
        {
            b_renderer->vertices[i] = vertices[i];
        }
    }
    XM_GLES2_BaseRenderer_uploadTexture(b_renderer, NULL);

    XM_GLES2_BaseRenderer_reloadShaderParameter(b_renderer);
    XM_Filter_ShaderVariables_reset(b_renderer->base_filter, shader_param);
    XM_Filter_setShaderVariables(b_renderer->base_filter, shader_param);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);		XM_GLES2_checkError_TRACE("glDrawArrays");
    return GL_TRUE;
}

static void XM_GLES2_BaseRenderer_TexCoords_reset(XM_GLES2_Base_Renderer *b_renderer)
{
    b_renderer->texcoords[0] = 0.0f;
    b_renderer->texcoords[1] = 0.0f;
    b_renderer->texcoords[2] = 1.0f;
    b_renderer->texcoords[3] = 0.0f;
    b_renderer->texcoords[4] = 0.0f;
    b_renderer->texcoords[5] = 1.0f;
    b_renderer->texcoords[6] = 1.0f;
    b_renderer->texcoords[7] = 1.0f;
}

static void XM_GLES2_BaseRenderer_Vertices_reset(XM_GLES2_Base_Renderer *b_renderer)
{
    b_renderer->vertices[0] = -1.0f;
    b_renderer->vertices[1] = -1.0f;
    b_renderer->vertices[2] =  1.0f;
    b_renderer->vertices[3] = -1.0f;
    b_renderer->vertices[4] = -1.0f;
    b_renderer->vertices[5] =  1.0f;
    b_renderer->vertices[6] =  1.0f;
    b_renderer->vertices[7] =  1.0f;
}

GLboolean XM_GLES2_BaseRenderer_use(XM_GLES2_Base_Renderer *b_renderer, SDL_VoutOverlay *overlay, XM_Filter_ShaderParameter *shader_param)
{
    GLboolean ret = GL_FALSE;
    if (!b_renderer)
        return ret;

    assert(b_renderer->func_use);
    if (!(ret = b_renderer->func_use(b_renderer, overlay)))
        return ret;

    XM_GLES2_BaseRenderer_Vertices_reset(b_renderer);
    XM_GLES2_BaseRenderer_TexCoords_reset(b_renderer);
    XM_GLES2_BaseRenderer_reloadShaderParameter(b_renderer);

    XM_Filter_ShaderVariables_reset(b_renderer->base_filter, shader_param);
    return ret;
}

GLboolean XM_GLES2_BaseRenderer_uploadTexture(XM_GLES2_Base_Renderer *b_renderer, SDL_VoutOverlay *overlay)
{
    GLboolean ret = GL_FALSE;
    if (!b_renderer)
        return ret;

    assert(b_renderer->func_uploadTexture);
    ret =  b_renderer->func_uploadTexture(b_renderer, overlay);

    return ret;
}

static XM_GLES2_Base_Renderer *XM_GLES2_BaseRenderer_create_base(const char *fsh, XM_Filter *filter)
{
    if(!filter || !fsh)
        return NULL;

    XM_GLES2_Base_Renderer *b_renderer = (XM_GLES2_Base_Renderer *)calloc(1, sizeof(XM_GLES2_Base_Renderer));
    if (!b_renderer)
        return NULL;

    const char *vsh = XM_Filter_getVertexShader(filter);
    if(!vsh)
        goto fail;

    b_renderer->index= filter->v_effects.baseType_index;
    b_renderer->show= filter->v_effects.baseType_show;
    b_renderer->base_filter = filter;
    XM_Filter_setRenderer(filter, b_renderer);

    b_renderer->vertex_shader = XM_GLES2_loadShader(GL_VERTEX_SHADER, vsh);
    if (!b_renderer->vertex_shader)
        goto fail;

    b_renderer->fragment_shader = XM_GLES2_loadShader(GL_FRAGMENT_SHADER, fsh);
    if (!b_renderer->fragment_shader)
        goto fail;

    b_renderer->program = glCreateProgram();                          XM_GLES2_checkError("glCreateProgram");
    if (!b_renderer->program)
        goto fail;

    glAttachShader(b_renderer->program, b_renderer->vertex_shader);     XM_GLES2_checkError("glAttachShader(vertex)");
    glAttachShader(b_renderer->program, b_renderer->fragment_shader);   XM_GLES2_checkError("glAttachShader(fragment)");
    glLinkProgram(b_renderer->program);                               XM_GLES2_checkError("glLinkProgram");
    GLint link_status = GL_FALSE;
    glGetProgramiv(b_renderer->program, GL_LINK_STATUS, &link_status);
    if (!link_status)
        goto fail;

    b_renderer->av4_position = glGetAttribLocation(b_renderer->program, "av4_Position");                XM_GLES2_checkError_TRACE("glGetAttribLocation(av4_Position)");
    b_renderer->av2_texcoord = glGetAttribLocation(b_renderer->program, "av2_Texcoord");                XM_GLES2_checkError_TRACE("glGetAttribLocation(av2_Texcoord)");
    b_renderer->um4_mvp      = glGetUniformLocation(b_renderer->program, "um4_ModelViewProjection");    XM_GLES2_checkError_TRACE("glGetUniformLocation(um4_ModelViewProjection)");

    XM_Filter_VertexShader_init(filter);
    return b_renderer;
fail:
    if (b_renderer->program)
        XM_GLES2_printProgramInfo(b_renderer->program);

    XM_GLES2_BaseRenderer_freeP(&b_renderer);
    return NULL;
}

XM_GLES2_Base_Renderer *XM_GLES2_BaseRenderer_create(SDL_VoutOverlay *overlay, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!overlay)
        return 0;

    ALOGI("create BEffects_Renderer\n");
    XM_Filter *filter = XM_Filter_Factory_createBaseFilters(overlay);
    XM_Filter_setBitmap(filter, xm_bmp);
    const char *fsh = XM_Filter_yuv420p_getFragmentShader(filter);

    XM_GLES2_Base_Renderer *b_renderer = XM_GLES2_BaseRenderer_create_base(fsh, filter);
    if(b_renderer == NULL)
        goto fail;

    b_renderer->us2_sampler = glGetUniformLocation(b_renderer->program, "us2_Sampler"); XM_GLES2_checkError_TRACE("glGetUniformLocation(us2_Sampler)");

    b_renderer->func_use = use;
    b_renderer->func_uploadTexture = uploadTexture;

    XM_Filter_FragmentShader_init(filter);
    if(b_renderer) {
        b_renderer->aspect_ratio = (double)overlay->w / (double)overlay->h;
        b_renderer->w = (double)overlay->w;
        b_renderer->h = (double)overlay->h;
    }
    return b_renderer;
fail:
    XM_GLES2_BaseRenderer_freeP(&b_renderer);
    return NULL;
}


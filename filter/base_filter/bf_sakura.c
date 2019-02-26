/*
* Created by sunyc on 2018-8-22.
* base filter sakura.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_sakura
{
    XM_GLES2_Base_Renderer *renderer;
    XM_Bitmap *xm_bmp;
    GLuint sampler2D[1];
    GLuint textures[1];
    GLubyte *pixels;
    GLuint texelWidthOffset;
    GLuint texelHeightOffset;
} XM_BFilter_sakura;

typedef struct XM_Filter_Opaque {
    XM_BFilter_sakura *filter;
} XM_Filter_Opaque;

static void load_texture(XM_BFilter_sakura *filter, int textures_num, int i, int w, int h, GLubyte *pixels)
{
    glActiveTexture(GL_TEXTURE0 + textures_num + i);
    glBindTexture(GL_TEXTURE_2D, filter->textures[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
                                0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glUniform1i(filter->sampler2D[i], textures_num + i);
}

static void pixels_malloc(XM_BFilter_sakura *filter)
{
    filter->pixels = (GLubyte *)malloc(1024);
    int arrayOfInt[256] = { 95, 95, 96, 97, 97, 98, 99, 99, 100, 101, 101, 102, 103, 104, 104, 105, 106, 106, 107, 108, 108, 109, 110, 111, 111, 112, 113, 113, 114, 115, 115, 116, 117, 117, 118, 119, 120, 120, 121, 122, 122, 123, 124, 124, 125, 126, 127, 127, 128, 129, 129, 130, 131, 131, 132, 133, 133, 134, 135, 136, 136, 137, 138, 138, 139, 140, 140, 141, 142, 143, 143, 144, 145, 145, 146, 147, 147, 148, 149, 149, 150, 151, 152, 152, 153, 154, 154, 155, 156, 156, 157, 158, 159, 159, 160, 161, 161, 162, 163, 163, 164, 165, 165, 166, 167, 168, 168, 169, 170, 170, 171, 172, 172, 173, 174, 175, 175, 176, 177, 177, 178, 179, 179, 180, 181, 181, 182, 183, 184, 184, 185, 186, 186, 187, 188, 188, 189, 190, 191, 191, 192, 193, 193, 194, 195, 195, 196, 197, 197, 198, 199, 200, 200, 201, 202, 202, 203, 204, 204, 205, 206, 207, 207, 208, 209, 209, 210, 211, 211, 212, 213, 213, 214, 215, 216, 216, 217, 218, 218, 219, 220, 220, 221, 222, 223, 223, 224, 225, 225, 226, 227, 227, 228, 229, 229, 230, 231, 232, 232, 233, 234, 234, 235, 236, 236, 237, 238, 239, 239, 240, 241, 241, 242, 243, 243, 244, 245, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
    for (int i = 0; i < 256; i++)
    {
        filter->pixels[(i * 4)] = arrayOfInt[i];
        filter->pixels[(1 + i * 4)] = arrayOfInt[i];
        filter->pixels[(2 + i * 4)] = arrayOfInt[i];
        filter->pixels[(3 + i * 4)] = arrayOfInt[i];
    }
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_sakura *filter = opaque->filter;
    glUniform1f(filter->texelWidthOffset, (1.0f / (float)filter->renderer->w));
    glUniform1f(filter->texelHeightOffset, (1.0f / (float)filter->renderer->h));
}

static bool yuv420p_uploadTexture(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    if (!opaque || !opaque->filter)
        return false;

    XM_BFilter_sakura *filter = opaque->filter;

    glBindTexture(GL_TEXTURE_2D, filter->textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, filter->pixels);
    return true;
}

//yuv420p_use must return true
static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    if (!opaque || !opaque->filter)
        return 0;

    XM_BFilter_sakura *filter = opaque->filter;
    pixels_malloc(filter);

    if (0 == filter->textures[0])
        glGenTextures(1, filter->textures);

    load_texture(filter, textures_num, 0, 256, 1, filter->pixels);
    return 1;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_sakura *filter = opaque->filter;
    filter->sampler2D[0] = glGetUniformLocation(filter->renderer->program, "curve"); XM_GLES2_checkError_TRACE("glGetUniformLocation(curve)");
    filter->texelWidthOffset = glGetUniformLocation(filter->renderer->program, "texelWidthOffset");
    filter->texelHeightOffset = glGetUniformLocation(filter->renderer->program, "texelHeightOffset");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
}

static void setBitmap(XM_Filter_Opaque *opaque, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_sakura *filter = opaque->filter;
    filter->xm_bmp = xm_bmp[V_BASE_EFFECTS_TYPE_SAKURA];
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_sakura *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_sakura();
}

static void XM_BFilter_reset(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_sakura *filter = opaque->filter;
    for (int i = 0; i < 1; ++i) {
        if (filter->textures[i]) {
            glDeleteTextures(1, &filter->textures[i]);
            filter->textures[i] = 0;
        }
    }

    if(filter->pixels)
    {
        free(filter->pixels);
    }
    filter->pixels = NULL;
}

static void XM_BFilter_Sakura_free(XM_BFilter_sakura *filter)
{
    if (!filter)
        return;

    if(filter->pixels)
    {
        free(filter->pixels);
    }
    filter->pixels = NULL;
    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_sakura));
    free(filter);
}

static void XM_BFilter_Sakura_freep(XM_BFilter_sakura **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_Sakura_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_Sakura_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_Sakura_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_sakura *)calloc(1, sizeof(XM_BFilter_sakura));

    filter->func_free = XM_BFilter_free;
    filter->func_reset = XM_BFilter_reset;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    //filter->func_setBitmap = setBitmap;
    filter->func_VertexShader_init = vsh_init;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_yuv420p_use = yuv420p_use;
    filter->func_yuv420p_uploadTexture= yuv420p_uploadTexture;
    filter->func_ShaderVariables_reset = shaderVariables_reset;
    filter->func_setShaderVariables = setShaderVariables;

    return filter;
}


/*
* Created by sunyc on 2018-8-22.
* base filter sun-set.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_sunset
{
    XM_GLES2_Base_Renderer *renderer;
    XM_Bitmap *xm_bmp;
    GLuint sampler2D[3];
    GLuint textures[3];
    GLubyte *pixels;
} XM_BFilter_sunset;

typedef struct XM_Filter_Opaque {
    XM_BFilter_sunset *filter;
} XM_Filter_Opaque;

static void load_texture(XM_BFilter_sunset *filter, int textures_num, int i, int w, int h, GLubyte *pixels)
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

static void pixels_malloc(XM_BFilter_sunset *filter)
{
    filter->pixels = (GLubyte *)malloc(2048);
    int arrayOfInt1[256] = { 0, 1, 2, 3, 5, 5, 7, 8, 9, 10, 11, 12, 13, 15, 16, 16, 18, 19, 20, 21, 23, 24, 25, 26, 27, 29, 30, 31, 32, 33, 35, 36, 38, 39, 40, 41, 42, 44, 45, 47, 48, 49, 51, 52, 54, 55, 56, 59, 60, 62, 63, 64, 66, 67, 70, 71, 72, 74, 76, 78, 79, 80, 83, 84, 85, 88, 89, 90, 93, 94, 95, 98, 99, 100, 102, 104, 106, 107, 108, 109, 112, 113, 114, 116, 117, 118, 119, 120, 122, 124, 125, 126, 128, 129, 130, 131, 132, 132, 133, 135, 136, 137, 138, 139, 140, 141, 142, 142, 143, 145, 146, 147, 148, 148, 149, 150, 151, 151, 152, 153, 154, 155, 155, 156, 157, 157, 158, 159, 160, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 167, 168, 169, 169, 170, 171, 171, 172, 173, 173, 174, 175, 175, 176, 177, 177, 178, 178, 179, 179, 180, 181, 181, 182, 183, 183, 184, 185, 185, 186, 187, 188, 188, 189, 190, 190, 191, 192, 193, 193, 194, 194, 194, 195, 196, 197, 197, 198, 199, 200, 201, 201, 202, 203, 204, 204, 205, 206, 207, 208, 208, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 217, 218, 218, 219, 220, 221, 222, 222, 223, 224, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 234, 235, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 247, 248, 248, 249, 250, 251, 252, 253, 254, 255 };
    int arrayOfInt2[256] = { 0, 1, 2, 3, 4, 5, 6, 7, 9, 9, 10, 12, 12, 13, 14, 16, 16, 17, 19, 20, 20, 22, 23, 24, 25, 26, 27, 29, 30, 31, 32, 33, 35, 36, 37, 39, 40, 41, 42, 43, 44, 46, 47, 49, 50, 51, 53, 54, 56, 57, 59, 61, 62, 64, 65, 66, 69, 70, 72, 73, 76, 77, 78, 80, 82, 84, 85, 87, 89, 90, 93, 94, 95, 98, 99, 100, 103, 104, 106, 108, 109, 111, 112, 114, 116, 117, 118, 120, 122, 123, 124, 125, 126, 129, 130, 131, 132, 133, 135, 136, 137, 138, 139, 140, 141, 142, 143, 145, 146, 147, 148, 149, 150, 151, 152, 152, 153, 154, 155, 156, 157, 158, 158, 159, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 167, 168, 169, 170, 170, 171, 172, 172, 173, 173, 174, 175, 175, 176, 177, 177, 178, 178, 178, 179, 179, 180, 181, 181, 182, 182, 183, 184, 184, 185, 185, 186, 187, 187, 188, 188, 189, 190, 190, 191, 191, 192, 193, 193, 194, 194, 194, 195, 195, 196, 197, 197, 198, 199, 199, 200, 201, 202, 202, 203, 204, 204, 205, 206, 207, 208, 208, 208, 209, 210, 210, 211, 212, 213, 214, 215, 215, 216, 217, 218, 219, 220, 221, 222, 222, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 234, 235, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 251, 252, 253, 254, 255 };
    int arrayOfInt3[256] = { 0, 1, 2, 3, 4, 5, 5, 7, 8, 9, 9, 11, 12, 12, 13, 14, 16, 16, 17, 18, 20, 20, 21, 22, 23, 25, 25, 26, 27, 29, 30, 31, 31, 32, 34, 35, 36, 37, 39, 40, 41, 41, 42, 44, 45, 46, 47, 48, 50, 51, 52, 53, 54, 56, 57, 59, 60, 61, 63, 64, 65, 66, 67, 69, 71, 72, 73, 74, 76, 78, 79, 80, 82, 83, 84, 85, 88, 89, 90, 92, 93, 94, 95, 98, 99, 100, 102, 103, 104, 106, 107, 108, 111, 112, 113, 114, 116, 117, 118, 119, 120, 122, 123, 124, 125, 126, 128, 129, 130, 131, 132, 133, 135, 136, 137, 138, 139, 140, 141, 142, 143, 145, 146, 147, 147, 148, 149, 150, 151, 152, 153, 154, 154, 155, 156, 157, 158, 159, 159, 160, 161, 162, 162, 163, 164, 165, 166, 166, 167, 168, 169, 169, 170, 171, 172, 172, 173, 174, 175, 175, 176, 177, 178, 178, 178, 179, 179, 180, 181, 182, 182, 183, 184, 185, 185, 186, 187, 188, 188, 189, 190, 191, 191, 192, 193, 194, 194, 194, 195, 196, 197, 198, 198, 199, 200, 201, 202, 203, 203, 204, 205, 206, 207, 208, 208, 209, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 222, 223, 224, 225, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 235, 236, 237, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 251, 252, 253, 254, 255 };
    int arrayOfInt4[256] = { 0, 1, 3, 4, 6, 7, 9, 10, 12, 13, 14, 16, 17, 19, 20, 21, 23, 24, 26, 27, 28, 30, 31, 32, 34, 35, 36, 38, 39, 40, 42, 43, 44, 45, 47, 48, 49, 51, 52, 53, 54, 55, 57, 58, 59, 60, 62, 63, 64, 65, 66, 67, 69, 70, 71, 72, 73, 74, 75, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 115, 116, 117, 118, 119, 120, 121, 121, 122, 123, 124, 125, 126, 126, 127, 128, 129, 130, 130, 131, 132, 133, 134, 135, 135, 136, 137, 138, 139, 140, 141, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 230, 231, 232, 233, 234, 235, 235, 236, 237, 238, 239, 239, 240, 241, 242, 243, 243, 244, 245, 245, 246, 247, 247, 248, 249, 249, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255 };
    for (int i = 0; i < 256; i++)
    {
        filter->pixels[(i * 4)] = arrayOfInt1[i];
        filter->pixels[(1 + i * 4)] = arrayOfInt2[i];
        filter->pixels[(2 + i * 4)] = arrayOfInt3[i];
        filter->pixels[(3 + i * 4)] = arrayOfInt4[i];
    }

    int arrayOfInt5[256] = { 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 25, 26, 26, 27, 27, 28, 28, 28, 28, 29, 29, 30, 29, 31, 31, 31, 31, 32, 32, 33, 33, 34, 34, 34, 34, 35, 35, 36, 36, 37, 37, 37, 38, 38, 39, 39, 39, 40, 40, 40, 41, 42, 42, 43, 43, 44, 44, 45, 45, 45, 46, 47, 47, 48, 48, 49, 50, 51, 51, 52, 52, 53, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 63, 63, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 90, 91, 93, 94, 95, 96, 97, 98, 100, 101, 103, 104, 105, 107, 108, 110, 111, 113, 115, 116, 118, 119, 120, 122, 123, 125, 127, 128, 130, 132, 134, 135, 137, 139, 141, 143, 144, 146, 148, 150, 152, 154, 156, 158, 160, 163, 165, 167, 169, 171, 173, 175, 178, 180, 182, 185, 187, 189, 192, 194, 197, 199, 201, 204, 206, 209, 211, 214, 216, 219, 221, 224, 226, 229, 232, 234, 236, 239, 241, 245, 247, 250, 252, 255 };
    for (int j = 0; j < 256; j++)
    {
        filter->pixels[(1024 + j * 4)] = arrayOfInt5[j];
        filter->pixels[(1 + (1024 + j * 4))] = arrayOfInt5[j];
        filter->pixels[(2 + (1024 + j * 4))] = arrayOfInt5[j];
        filter->pixels[(3 + (1024 + j * 4))] = -1;
    }
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
}

static bool yuv420p_uploadTexture(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    if (!opaque || !opaque->filter)
        return false;

    XM_BFilter_sunset *filter = opaque->filter;

    glBindTexture(GL_TEXTURE_2D, filter->textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 2,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, filter->pixels);

    for(int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, filter->textures[i + 1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, filter->xm_bmp[i].w, filter->xm_bmp[i].h,
                            0, GL_RGBA, GL_UNSIGNED_BYTE, filter->xm_bmp[i].pBitmap);
    }
    return true;
}

//yuv420p_use must return true
static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    if (!opaque || !opaque->filter)
        return 0;

    XM_BFilter_sunset *filter = opaque->filter;
    pixels_malloc(filter);

    if (0 == filter->textures[0])
        glGenTextures(3, filter->textures);

    load_texture(filter, textures_num, 0, 256, 2, filter->pixels);
    for(int i = 0; i < 2; i++)
        load_texture(filter, textures_num, i + 1, filter->xm_bmp[i].w, filter->xm_bmp[i].h, filter->xm_bmp[i].pBitmap);

    return 1;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_sunset *filter = opaque->filter;
    filter->sampler2D[0] = glGetUniformLocation(filter->renderer->program, "curve"); XM_GLES2_checkError_TRACE("glGetUniformLocation(curve)");
    filter->sampler2D[1] = glGetUniformLocation(filter->renderer->program, "grey1Frame");
    filter->sampler2D[2] = glGetUniformLocation(filter->renderer->program, "grey2Frame");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
}

static void setBitmap(XM_Filter_Opaque *opaque, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_sunset *filter = opaque->filter;
    filter->xm_bmp = xm_bmp[V_BASE_EFFECTS_TYPE_SUNSET];
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_sunset *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_sunset();
}

static void XM_BFilter_reset(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_sunset *filter = opaque->filter;
    for (int i = 0; i < 3; ++i) {
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

static void XM_BFilter_SunSet_free(XM_BFilter_sunset *filter)
{
    if (!filter)
        return;

    if(filter->pixels)
    {
        free(filter->pixels);
    }
    filter->pixels = NULL;
    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_sunset));
    free(filter);
}

static void XM_BFilter_SunSet_freep(XM_BFilter_sunset **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_SunSet_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_SunSet_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_SunSet_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_sunset *)calloc(1, sizeof(XM_BFilter_sunset));

    filter->func_free = XM_BFilter_free;
    filter->func_reset = XM_BFilter_reset;
    filter->func_yuv420p_getFragmentShader = yuv420p_getFragmentShader;
    filter->func_getVertexShader = getVertexShader;
    filter->func_setRenderer = setRenderer;
    filter->func_setBitmap = setBitmap;
    filter->func_VertexShader_init = vsh_init;
    filter->func_FragmentShader_init = fsh_init;
    filter->func_yuv420p_use = yuv420p_use;
    filter->func_yuv420p_uploadTexture= yuv420p_uploadTexture;
    filter->func_ShaderVariables_reset = shaderVariables_reset;
    filter->func_setShaderVariables = setShaderVariables;

    return filter;
}


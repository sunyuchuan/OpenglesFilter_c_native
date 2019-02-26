/*
* Created by sunyc on 2018-8-22.
* base filter whitecat.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"

typedef struct XM_BFilter_whitecat
{
    XM_GLES2_Base_Renderer *renderer;
    XM_Bitmap *xm_bmp;
    GLuint sampler2D[1];
    GLuint textures[1];
    GLubyte *pixels;
} XM_BFilter_whitecat;

typedef struct XM_Filter_Opaque {
    XM_BFilter_whitecat *filter;
} XM_Filter_Opaque;

static void load_texture(XM_BFilter_whitecat *filter, int textures_num, int i, int w, int h, GLubyte *pixels)
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

static void pixels_malloc(XM_BFilter_whitecat *filter)
{
    filter->pixels = (GLubyte *)malloc(2048);

    int arrayOfInt1[256] = { 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 16, 17, 18, 19, 20, 22, 23, 24, 25, 26, 28, 29, 30, 31, 32, 33, 35, 36, 37, 38, 39, 41, 42, 43, 44, 45, 46, 48, 49, 50, 51, 52, 54, 55, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 67, 69, 70, 71, 72, 73, 74, 75, 77, 78, 79, 80, 81, 82, 83, 85, 86, 87, 88, 89, 90, 91, 92, 93, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 249, 250, 251, 252, 253, 254, 255, 255, 255, 255, 255, 255 };
    int arrayOfInt2[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 4, 5, 6, 7, 8, 10, 11, 12, 12, 13, 14, 16, 17, 18, 19, 19, 20, 22, 23, 24, 25, 26, 26, 28, 29, 30, 31, 32, 33, 35, 35, 36, 37, 38, 39, 41, 42, 42, 43, 44, 45, 46, 48, 49, 50, 50, 51, 52, 54, 55, 56, 57, 58, 58, 59, 61, 62, 63, 64, 65, 66, 66, 67, 69, 70, 71, 72, 73, 74, 75, 75, 77, 78, 79, 80, 81, 82, 83, 85, 85, 86, 87, 88, 89, 90, 91, 92, 93, 93, 95, 96, 97, 98, 99, 100, 101, 102, 103, 103, 104, 105, 107, 108, 109, 110, 111, 112, 113, 114, 114, 115, 116, 117, 118, 119, 120, 121, 123, 124, 125, 126, 127, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 182, 183, 184, 185, 186, 187, 188, 189, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 217, 218, 219, 220, 221, 222, 223, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 236, 237, 238, 239, 240, 240 };
    for (int i = 0; i < 256; i++){
        filter->pixels[(i * 4)] = arrayOfInt1[i];
        filter->pixels[(1 + i * 4)] = arrayOfInt1[i];
        filter->pixels[(2 + i * 4)] = arrayOfInt2[i];
        filter->pixels[(3 + i * 4)] = -1;
    }
    int arrayOfInt3[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 6, 9, 14, 17, 19, 22, 25, 27, 30, 34, 36, 39, 41, 43, 45, 49, 51, 52, 54, 55, 57, 58, 61, 63, 64, 65, 67, 68, 69, 72, 73, 75, 76, 77, 78, 81, 82, 83, 84, 86, 87, 88, 90, 91, 93, 94, 95, 96, 97, 99, 100, 101, 102, 103, 105, 106, 108, 109, 110, 111, 112, 113, 115, 116, 117, 118, 119, 120, 121, 123, 124, 125, 126, 126, 127, 128, 130, 131, 132, 133, 134, 135, 136, 138, 138, 139, 140, 141, 142, 144, 145, 146, 146, 147, 148, 149, 151, 152, 153, 153, 154, 155, 156, 158, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 168, 170, 171, 172, 172, 173, 174, 175, 176, 177, 178, 179, 180, 180, 181, 183, 183, 184, 185, 186, 186, 188, 189, 190, 190, 191, 192, 193, 194, 195, 196, 196, 197, 198, 199, 200, 201, 201, 202, 203, 204, 204, 206, 207, 207, 208, 209, 209, 211, 212, 212, 213, 214, 214, 215, 217, 217, 218, 219, 219, 220, 221, 222, 223, 224, 224, 225, 226, 227, 228, 228, 229, 230, 230, 231, 233, 233, 234, 235, 235, 236, 237, 238, 239, 239, 240, 241, 241, 242, 243, 244, 245, 245, 246, 247, 248, 249, 249, 250, 250, 251, 252, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
    int arrayOfInt4[256] = { 0, 2, 4, 6, 8, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 32, 34, 36, 38, 40, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 60, 61, 63, 65, 66, 68, 70, 71, 73, 74, 76, 77, 79, 80, 82, 83, 85, 86, 88, 89, 91, 92, 93, 95, 96, 98, 99, 100, 101, 103, 104, 105, 107, 108, 109, 110, 111, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 124, 125, 126, 127, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 198, 199, 200, 201, 202, 203, 204, 205, 206, 206, 207, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 216, 217, 218, 219, 219, 220, 221, 222, 222, 223, 224, 224, 225, 226, 226, 227, 228, 228, 229, 230, 230, 231, 232, 232, 233, 233, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 245, 246, 246, 247, 247, 247, 248, 248, 248, 249, 249, 249, 250, 250, 250, 251, 251, 251, 252, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 254, 254, 255, 255, 255 };
    int arrayOfInt5[256] = { 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 25, 26, 26, 27, 27, 28, 28, 28, 28, 29, 29, 30, 29, 31, 31, 31, 31, 32, 32, 33, 33, 34, 34, 34, 34, 35, 35, 36, 36, 37, 37, 37, 38, 38, 39, 39, 39, 40, 40, 40, 41, 42, 42, 43, 43, 44, 44, 45, 45, 45, 46, 47, 47, 48, 48, 49, 50, 51, 51, 52, 52, 53, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 63, 63, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 90, 91, 93, 94, 95, 96, 97, 98, 100, 101, 103, 104, 105, 107, 108, 110, 111, 113, 115, 116, 118, 119, 120, 122, 123, 125, 127, 128, 130, 132, 134, 135, 137, 139, 141, 143, 144, 146, 148, 150, 152, 154, 156, 158, 160, 163, 165, 167, 169, 171, 173, 175, 178, 180, 182, 185, 187, 189, 192, 194, 197, 199, 201, 204, 206, 209, 211, 214, 216, 219, 221, 224, 226, 229, 232, 234, 236, 239, 241, 245, 247, 250, 252, 255 };
    for (int j = 0; j < 256; j++){
        filter->pixels[(1024 + j * 4)] = arrayOfInt4[j];
        filter->pixels[(1 + (1024 + j * 4))] = arrayOfInt3[j];
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

    XM_BFilter_whitecat *filter = opaque->filter;

    glBindTexture(GL_TEXTURE_2D, filter->textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 2,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, filter->pixels);

    return true;
}

//yuv420p_use must return true
static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    if (!opaque || !opaque->filter)
        return 0;

    XM_BFilter_whitecat *filter = opaque->filter;
    pixels_malloc(filter);

    if (0 == filter->textures[0])
        glGenTextures(1, filter->textures);

    load_texture(filter, textures_num, 0, 256, 2, filter->pixels);
    return 1;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_whitecat *filter = opaque->filter;
    filter->sampler2D[0] = glGetUniformLocation(filter->renderer->program, "curve"); XM_GLES2_checkError_TRACE("glGetUniformLocation(curve)");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_whitecat *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static void setBitmap(XM_Filter_Opaque *opaque, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_whitecat *filter = opaque->filter;
    filter->xm_bmp = xm_bmp[V_BASE_EFFECTS_TYPE_WHITECAT];
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_default();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_whitecat();
}

static void XM_BFilter_reset(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_whitecat *filter = opaque->filter;
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

static void XM_BFilter_WhiteCat_free(XM_BFilter_whitecat *filter)
{
    if (!filter)
        return;

    if(filter->pixels)
    {
        free(filter->pixels);
    }
    filter->pixels = NULL;
    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_whitecat));
    free(filter);
}

static void XM_BFilter_WhiteCat_freep(XM_BFilter_whitecat **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_WhiteCat_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_WhiteCat_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_WhiteCat_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_whitecat *)calloc(1, sizeof(XM_BFilter_whitecat));

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


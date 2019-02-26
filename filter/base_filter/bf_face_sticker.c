/*
* Created by sunyc on 2018-10-8.
* time filter face sticker.
*/
#include "base_filter/base_renderer.h"
#include "base_filter/base_filter.h"
#include "ijksdl/xm_bitmap.h"

typedef struct XM_BFilter_FaceSticker
{
    XM_GLES2_Base_Renderer *renderer;
    XM_Bitmap *xm_bmp;
    GLuint inputImageTexture2;
    GLuint textures[1];
    GLuint mLeftTop;
    GLuint mRightBottom;
    GLfloat mRectangleCoordinates[4];
    GLfloat mOffScreen;
} XM_BFilter_FaceSticker;

typedef struct XM_Filter_Opaque {
    XM_BFilter_FaceSticker *filter;
} XM_Filter_Opaque;

static void normalized(int x1, int y1, int x2, int y2, int w, int h, float dstBuffer[4])
{
    if(!dstBuffer)
        return;

    dstBuffer[0] = ((float)x1 / (float)w) * 2 - 1;
    dstBuffer[1] = ((float)y1 / (float)h) * 2 - 1;
    dstBuffer[2] = ((float)x2 / (float)w) * 2 - 1;
    dstBuffer[3] = ((float)y2 / (float)h) * 2 - 1;
}

static void shaderVariables_reset(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter || !param)
        return;

    XM_BFilter_FaceSticker *filter = opaque->filter;
    int x1 = param->face_rectangle_coordinates[0];
    int y1 = param->face_rectangle_coordinates[1];
    int x2 = param->face_rectangle_coordinates[2];
    int y2 = param->face_rectangle_coordinates[3];
    int w = param->face_rectangle_coordinates[4];
    int h = param->face_rectangle_coordinates[5];

    if(param->off_screen) {
        glUniform1f(filter->mOffScreen, 1.0);
        normalized(x1, y1, x2, y2, w, h, filter->mRectangleCoordinates);
    } else {
        glUniform1f(filter->mOffScreen, -1.0);
        normalized(x1, h - y2, x2, h - y1, w, h, filter->mRectangleCoordinates);
    }

    glUniform2fv(filter->mLeftTop, 1, filter->mRectangleCoordinates);
    glUniform2fv(filter->mRightBottom, 1, filter->mRectangleCoordinates + 2);
}

static void setShaderVariables(XM_Filter_Opaque *opaque, XM_Filter_ShaderParameter *param)
{
    if (!opaque || !opaque->filter || !param)
        return;

    XM_BFilter_FaceSticker *filter = opaque->filter;
    int x1 = param->face_rectangle_coordinates[0];
    int y1 = param->face_rectangle_coordinates[1];
    int x2 = param->face_rectangle_coordinates[2];
    int y2 = param->face_rectangle_coordinates[3];
    int w = param->face_rectangle_coordinates[4];
    int h = param->face_rectangle_coordinates[5];

    if(param->off_screen) {
        normalized(x1, y1, x2, y2, w, h, filter->mRectangleCoordinates);
    } else {
        normalized(x1, h - y2, x2, h - y1, w, h, filter->mRectangleCoordinates);
    }

    glUniform2fv(filter->mLeftTop, 1, filter->mRectangleCoordinates);
    glUniform2fv(filter->mRightBottom, 1, filter->mRectangleCoordinates + 2);
}

static bool yuv420p_uploadTexture(XM_Filter_Opaque *opaque, SDL_VoutOverlay *overlay)
{
    if (!opaque || !opaque->filter)
        return 0;

    XM_BFilter_FaceSticker *filter = opaque->filter;

    glBindTexture(GL_TEXTURE_2D, filter->textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, filter->xm_bmp[0].w, filter->xm_bmp[0].h,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, filter->xm_bmp[0].pBitmap);
    return true;
}

//yuv420p_use must return 1
static int yuv420p_use(XM_Filter_Opaque *opaque, int textures_num)
{
    if (!opaque || !opaque->filter)
        return 0;

    XM_BFilter_FaceSticker *filter = opaque->filter;

    if (0 == filter->textures[0])
        glGenTextures(1, filter->textures);

    glActiveTexture(GL_TEXTURE0 + textures_num);
    glBindTexture(GL_TEXTURE_2D, filter->textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, filter->xm_bmp[0].w, filter->xm_bmp[0].h,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, filter->xm_bmp[0].pBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1i(filter->inputImageTexture2, textures_num);
    return 1;
}

static void fsh_init(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_FaceSticker *filter = opaque->filter;
    filter->inputImageTexture2 = glGetUniformLocation(filter->renderer->program, "inputImageTexture2"); XM_GLES2_checkError_TRACE("glGetUniformLocation(inputImageTexture2)");
    filter->mLeftTop = glGetUniformLocation(filter->renderer->program, "leftTop"); XM_GLES2_checkError_TRACE("glGetUniformLocation(leftTop)");
    filter->mRightBottom = glGetUniformLocation(filter->renderer->program, "rightBottom"); XM_GLES2_checkError_TRACE("glGetUniformLocation(rightBottom)");
    filter->mOffScreen = glGetUniformLocation(filter->renderer->program, "offScreen"); XM_GLES2_checkError_TRACE("glGetUniformLocation(offScreen)");
}

static void vsh_init(XM_Filter_Opaque *opaque)
{
}

static void setRenderer(XM_Filter_Opaque *opaque, void *renderer)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_FaceSticker *filter = opaque->filter;
    filter->renderer = (XM_GLES2_Base_Renderer *)renderer;
}

static void setBitmap(XM_Filter_Opaque *opaque, XM_Bitmap xm_bmp[][EGL_BITMAP_NUMBER_MAX])
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_FaceSticker *filter = opaque->filter;
    filter->xm_bmp = xm_bmp[V_BASE_EFFECTS_TYPE_FACE_STICKER];
}

static const char *getVertexShader()
{
    return XM_GLES2_getVertexShader_face_sticker();
}

static const char *yuv420p_getFragmentShader()
{
    return XM_BFilter_getFragmentShader_face_sticker();
}

static void XM_BFilter_reset(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_FaceSticker *filter = opaque->filter;
    for (int i = 0; i < 1; ++i) {
        if (filter->textures[i]) {
            glDeleteTextures(1, &filter->textures[i]);
            filter->textures[i] = 0;
        }
    }
}

static void XM_BFilter_FaceSticker_free(XM_BFilter_FaceSticker *filter)
{
    if (!filter)
        return;

    //XM_TFilter_Fission_terminate(filter->opaque);
    memset(filter, 0, sizeof(XM_BFilter_FaceSticker));
    free(filter);
}

static void XM_BFilter_FaceSticker_freep(XM_BFilter_FaceSticker **filter)
{
    if (!filter || !*filter)
        return;

    XM_BFilter_FaceSticker_free(*filter);
    *filter = NULL;
}

static void XM_BFilter_free(XM_Filter_Opaque *opaque)
{
    if (!opaque || !opaque->filter)
        return;

    XM_BFilter_FaceSticker_freep(&opaque->filter);
}

XM_Filter *XM_BFilter_FaceSticker_create()
{
    XM_Filter *filter = XM_Filter_create(sizeof(XM_Filter_Opaque));
    if (!filter)
        return NULL;

    XM_Filter_Opaque *opaque = filter->opaque;
    opaque->filter = (XM_BFilter_FaceSticker *)calloc(1, sizeof(XM_BFilter_FaceSticker));

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


/*
 * Created by sunyc on 2018-6-25.
 * Create Vertex shaders of 3x3 texture sampling effects
 */

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_3x3texture[] = XM_GLES_STRING(
    precision highp float;
    attribute highp vec4 av4_Position;
    attribute highp vec2 av2_Texcoord;
    uniform         mat4 um4_ModelViewProjection;

    uniform highp float texelWidth;
    uniform highp float texelHeight;

    varying vec2 vv2_Texcoord;
    varying vec2 leftTextureCoordinate;
    varying vec2 rightTextureCoordinate;

    varying vec2 topTextureCoordinate;
    varying vec2 topLeftTextureCoordinate;
    varying vec2 topRightTextureCoordinate;

    varying vec2 bottomTextureCoordinate;
    varying vec2 bottomLeftTextureCoordinate;
    varying vec2 bottomRightTextureCoordinate;

    void main()
    {
        gl_Position = um4_ModelViewProjection * av4_Position;

        vec2 widthStep = vec2(texelWidth, 0.0);
        vec2 heightStep = vec2(0.0, texelHeight);
        vec2 widthHeightStep = vec2(texelWidth, texelHeight);
        vec2 widthNegativeHeightStep = vec2(texelWidth, -texelHeight);

        vv2_Texcoord = av2_Texcoord.xy;
        leftTextureCoordinate = av2_Texcoord.xy - widthStep;
        rightTextureCoordinate = av2_Texcoord.xy + widthStep;

        topTextureCoordinate = av2_Texcoord.xy - heightStep;
        topLeftTextureCoordinate = av2_Texcoord.xy - widthHeightStep;
        topRightTextureCoordinate = av2_Texcoord.xy + widthNegativeHeightStep;

        bottomTextureCoordinate = av2_Texcoord.xy + heightStep;
        bottomLeftTextureCoordinate = av2_Texcoord.xy - widthNegativeHeightStep;
        bottomRightTextureCoordinate = av2_Texcoord.xy + widthHeightStep;
    }
);

const char *XM_GLES2_BaseFilter_getVertexShader_3x3texture()
{
    return g_shader_3x3texture;
}


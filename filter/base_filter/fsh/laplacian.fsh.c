/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of laplacian effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_laplacian[] = XM_GLES_STRING(
    precision highp float;
    varying highp vec2 vv2_Texcoord;
    uniform lowp sampler2D us2_Sampler;

    uniform mediump mat3 convolutionMatrix;

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
        mediump vec3 bottomColor = texture2D(us2_Sampler, bottomTextureCoordinate).rgb;
        mediump vec3 bottomLeftColor = texture2D(us2_Sampler, bottomLeftTextureCoordinate).rgb;
        mediump vec3 bottomRightColor = texture2D(us2_Sampler, bottomRightTextureCoordinate).rgb;
        mediump vec4 centerColor = texture2D(us2_Sampler, vv2_Texcoord);
        mediump vec3 leftColor = texture2D(us2_Sampler, leftTextureCoordinate).rgb;
        mediump vec3 rightColor = texture2D(us2_Sampler, rightTextureCoordinate).rgb;
        mediump vec3 topColor = texture2D(us2_Sampler, topTextureCoordinate).rgb;
        mediump vec3 topRightColor = texture2D(us2_Sampler, topRightTextureCoordinate).rgb;
        mediump vec3 topLeftColor = texture2D(us2_Sampler, topLeftTextureCoordinate).rgb;

        mediump vec3 resultColor = topLeftColor * convolutionMatrix[0][0] + topColor * convolutionMatrix[0][1] + topRightColor * convolutionMatrix[0][2];
        resultColor += leftColor * convolutionMatrix[1][0] + centerColor.rgb * convolutionMatrix[1][1] + rightColor * convolutionMatrix[1][2];
        resultColor += bottomLeftColor * convolutionMatrix[2][0] + bottomColor * convolutionMatrix[2][1] + bottomRightColor * convolutionMatrix[2][2];

        // Normalize the results to allow for negative gradients in the 0.0-1.0 colorspace
        resultColor = resultColor + 0.5;

        gl_FragColor = vec4(resultColor, centerColor.a);
    }
);

const char *XM_BFilter_getFragmentShader_laplacian()
{
    return g_shader_laplacian;
}


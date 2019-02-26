/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of sobel edge detection effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_edge_detection[] = XM_GLES_STRING(
    precision highp float;
    varying highp vec2 vv2_Texcoord;
    uniform lowp sampler2D us2_Sampler;

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
        float bottomLeftIntensity = texture2D(us2_Sampler, bottomLeftTextureCoordinate).r;
        float topRightIntensity = texture2D(us2_Sampler, topRightTextureCoordinate).r;
        float topLeftIntensity = texture2D(us2_Sampler, topLeftTextureCoordinate).r;
        float bottomRightIntensity = texture2D(us2_Sampler, bottomRightTextureCoordinate).r;
        float leftIntensity = texture2D(us2_Sampler, leftTextureCoordinate).r;
        float rightIntensity = texture2D(us2_Sampler, rightTextureCoordinate).r;
        float bottomIntensity = texture2D(us2_Sampler, bottomTextureCoordinate).r;
        float topIntensity = texture2D(us2_Sampler, topTextureCoordinate).r;

        vec2 gradientDirection;
        gradientDirection.x = -bottomLeftIntensity - 2.0 * leftIntensity - topLeftIntensity + bottomRightIntensity + 2.0 * rightIntensity + topRightIntensity;
        gradientDirection.y = -topLeftIntensity - 2.0 * topIntensity - topRightIntensity + bottomLeftIntensity + 2.0 * bottomIntensity + bottomRightIntensity;

        float gradientMagnitude = length(gradientDirection);
        vec2 normalizedDirection = normalize(gradientDirection);
        normalizedDirection = sign(normalizedDirection) * floor(abs(normalizedDirection) + 0.617316); // Offset by 1-sin(pi/8) to set to 0 if near axis, 1 if away
        normalizedDirection = (normalizedDirection + 1.0) * 0.5; // Place -1.0 - 1.0 within 0 - 1.0
        gl_FragColor = vec4(gradientMagnitude, normalizedDirection.x, normalizedDirection.y, 1.0);
    }
);

const char *XM_BFilter_getFragmentShader_edge_detection()
{
    return g_shader_edge_detection;
}


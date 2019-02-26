/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of swirl effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_swirl[] = XM_GLES_STRING(
    precision highp float;
    varying highp vec2 vv2_Texcoord;
    uniform lowp sampler2D us2_Sampler;

    uniform highp vec2 center;
    uniform highp float radius;
    uniform highp float angle;

    void main()
    {
        highp vec2 textureCoordinateToUse = vv2_Texcoord;
        highp float dist = distance(center, vv2_Texcoord);
        if (dist < radius)
        {
            textureCoordinateToUse -= center;
            highp float percent = (radius - dist) / radius;
            highp float theta = percent * percent * angle * 8.0;
            highp float s = sin(theta);
            highp float c = cos(theta);
            textureCoordinateToUse = vec2(dot(textureCoordinateToUse, vec2(c, -s)), dot(textureCoordinateToUse, vec2(s, c)));
            textureCoordinateToUse += center;
        }

        gl_FragColor = texture2D(us2_Sampler, textureCoordinateToUse);
    }
);

const char *XM_BFilter_getFragmentShader_swirl()
{
    return g_shader_swirl;
}


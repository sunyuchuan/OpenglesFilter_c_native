/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of retro effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_retro[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    uniform   highp vec3 vv3_ColorRatio;
    uniform   lowp  sampler2D us2_Sampler;

    void main()
    {
        lowp    vec4 rgba;
        highp float temp;

        rgba = texture2D(us2_Sampler, vv2_Texcoord);
        temp = rgba.r * vv3_ColorRatio.r + rgba.g * vv3_ColorRatio.g + rgba.b * vv3_ColorRatio.b;
        gl_FragColor = vec4(temp, temp, temp, 1);
    }
);

const char *XM_BFilter_getFragmentShader_retro()
{
    return g_shader_retro;
}


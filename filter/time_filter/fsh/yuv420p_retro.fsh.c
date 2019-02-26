/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of retro effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_retro[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    uniform   highp vec3 vv3_ColorRatio;
    uniform         mat3 um3_ColorConversion;
    uniform   lowp  sampler2D us2_SamplerX;
    uniform   lowp  sampler2D us2_SamplerY;
    uniform   lowp  sampler2D us2_SamplerZ;

    void main()
    {
        mediump vec3 yuv;
        lowp    vec3 rgb;
        highp float temp;

        yuv.x = (texture2D(us2_SamplerX, vv2_Texcoord).r - (16.0 / 255.0));
        yuv.y = (texture2D(us2_SamplerY, vv2_Texcoord).r - 0.5);
        yuv.z = (texture2D(us2_SamplerZ, vv2_Texcoord).r - 0.5);

        rgb = um3_ColorConversion * yuv;
        temp = rgb.r * vv3_ColorRatio.r + rgb.g * vv3_ColorRatio.g + rgb.b * vv3_ColorRatio.b;
        gl_FragColor = vec4(temp, temp, temp, 1);
    }
);

const char *XM_TFilter_yuv420p_getFragmentShader_retro()
{
    return g_shader_retro;
}


/*
* Created by sunyc on 2018-6-25.
* Create Vertex shaders of soul effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_ghost[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    varying   highp vec2 vv2_Texcoord_ghost;
    uniform         mat3 um3_ColorConversion;
    uniform   lowp  sampler2D us2_SamplerX;
    uniform   lowp  sampler2D us2_SamplerY;
    uniform   lowp  sampler2D us2_SamplerZ;
    uniform   lowp  sampler2D us2_SamplerX_ghost;
    uniform   lowp  sampler2D us2_SamplerY_ghost;
    uniform   lowp  sampler2D us2_SamplerZ_ghost;

    void main()
    {
        mediump vec3 yuv;
        mediump vec3 yuv_ghost;
        lowp    vec3 rgb;

        yuv.x = (texture2D(us2_SamplerX, vv2_Texcoord).r - (16.0 / 255.0));
        yuv.y = (texture2D(us2_SamplerY, vv2_Texcoord).r - 0.5);
        yuv.z = (texture2D(us2_SamplerZ, vv2_Texcoord).r - 0.5);

        yuv_ghost.x = (texture2D(us2_SamplerX_ghost, vv2_Texcoord_ghost).r - (16.0 / 255.0));
        yuv_ghost.y = (texture2D(us2_SamplerY_ghost, vv2_Texcoord_ghost).r - 0.5);
        yuv_ghost.z = (texture2D(us2_SamplerZ_ghost, vv2_Texcoord_ghost).r - 0.5);

        rgb = um3_ColorConversion * mix(yuv, yuv_ghost, 0.2);
        gl_FragColor = vec4(rgb, 1);
    }
);

const char *XM_TFilter_yuv420p_getFragmentShader_ghost()
{
    return g_shader_ghost;
}


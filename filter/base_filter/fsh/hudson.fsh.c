/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of hudson effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_hudson[] = XM_GLES_STRING(
        precision highp float;
        varying highp vec2 vv2_Texcoord;
        uniform lowp sampler2D us2_Sampler;

        uniform sampler2D inputImageTexture2; //blowout;
        uniform sampler2D inputImageTexture3; //overlay;
        uniform sampler2D inputImageTexture4; //map
        uniform float strength;

void main()
{
    vec4 originColor = texture2D(us2_Sampler, vv2_Texcoord);

    vec4 texel = texture2D(us2_Sampler, vv2_Texcoord);

    vec3 bbTexel = texture2D(inputImageTexture2, vv2_Texcoord).rgb;

    texel.r = texture2D(inputImageTexture3, vec2(bbTexel.r, texel.r)).r;
    texel.g = texture2D(inputImageTexture3, vec2(bbTexel.g, texel.g)).g;
    texel.b = texture2D(inputImageTexture3, vec2(bbTexel.b, texel.b)).b;

    vec4 mapped;
    mapped.r = texture2D(inputImageTexture4, vec2(texel.r, .16666)).r;
    mapped.g = texture2D(inputImageTexture4, vec2(texel.g, .5)).g;
    mapped.b = texture2D(inputImageTexture4, vec2(texel.b, .83333)).b;
    mapped.a = 1.0;

    mapped.rgb = mix(originColor.rgb, mapped.rgb, strength);

    gl_FragColor = mapped;
}
);

const char *XM_BFilter_getFragmentShader_hudson()
{
    return g_shader_hudson;
}


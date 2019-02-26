/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of brannan effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_brannan[] = XM_GLES_STRING(
        precision highp float;
        varying highp vec2 vv2_Texcoord;
        uniform lowp sampler2D us2_Sampler;

        uniform lowp sampler2D inputImageTexture2;  //process
        uniform lowp sampler2D inputImageTexture3;  //blowout
        uniform lowp sampler2D inputImageTexture4;  //contrast
        uniform lowp sampler2D inputImageTexture5;  //luma
        uniform lowp sampler2D inputImageTexture6;  //screen

        mat3 saturateMatrix = mat3(
                                  1.105150, -0.044850,-0.046000,
                                  -0.088050,1.061950,-0.089200,
                                  -0.017100,-0.017100,1.132900);
        vec3 luma = vec3(.3, .59, .11);
        uniform float strength;

void main()
{
    vec4 originColor = texture2D(us2_Sampler, vv2_Texcoord);
    vec3 texel = texture2D(us2_Sampler, vv2_Texcoord).rgb;

    vec2 lookup;
    lookup.y = 0.5;
    lookup.x = texel.r;
    texel.r = texture2D(inputImageTexture2, lookup).r;
    lookup.x = texel.g;
    texel.g = texture2D(inputImageTexture2, lookup).g;
    lookup.x = texel.b;
    texel.b = texture2D(inputImageTexture2, lookup).b;

    texel = saturateMatrix * texel;


    vec2 tc = (2.0 * vv2_Texcoord) - 1.0;
    float d = dot(tc, tc);
    vec3 sampled;
    lookup.y = 0.5;
    lookup.x = texel.r;
    sampled.r = texture2D(inputImageTexture3, lookup).r;
    lookup.x = texel.g;
    sampled.g = texture2D(inputImageTexture3, lookup).g;
    lookup.x = texel.b;
    sampled.b = texture2D(inputImageTexture3, lookup).b;
    float value = smoothstep(0.0, 1.0, d);
    texel = mix(sampled, texel, value);

    lookup.x = texel.r;
    texel.r = texture2D(inputImageTexture4, lookup).r;
    lookup.x = texel.g;
    texel.g = texture2D(inputImageTexture4, lookup).g;
    lookup.x = texel.b;
    texel.b = texture2D(inputImageTexture4, lookup).b;


    lookup.x = dot(texel, luma);
    texel = mix(texture2D(inputImageTexture5, lookup).rgb, texel, .5);

    lookup.x = texel.r;
    texel.r = texture2D(inputImageTexture6, lookup).r;
    lookup.x = texel.g;
    texel.g = texture2D(inputImageTexture6, lookup).g;
    lookup.x = texel.b;
    texel.b = texture2D(inputImageTexture6, lookup).b;

    texel = mix(originColor.rgb, texel.rgb, strength);

    gl_FragColor = vec4(texel, 1.0);
}
);

const char *XM_BFilter_getFragmentShader_brannan()
{
    return g_shader_brannan;
}


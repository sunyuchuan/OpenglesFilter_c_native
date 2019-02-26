/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of hefe effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_hefe[] = XM_GLES_STRING(
                                        precision highp float;
                                        varying highp vec2 vv2_Texcoord;
                                        uniform lowp sampler2D us2_Sampler;

                                        uniform sampler2D inputImageTexture2;  //edgeBurn
                                        uniform sampler2D inputImageTexture3;  //hefeMap
                                        uniform sampler2D inputImageTexture4;  //hefeGradientMap
                                        uniform sampler2D inputImageTexture5;  //hefeSoftLight
                                        uniform sampler2D inputImageTexture6;  //hefeMetal
                                        uniform float strength;

void main()
{
    vec4 originColor = texture2D(us2_Sampler, vv2_Texcoord);
    vec3 texel = texture2D(us2_Sampler, vv2_Texcoord).rgb;
    vec3 edge = texture2D(inputImageTexture2, vv2_Texcoord).rgb;
    texel = texel * edge;

    texel = vec3(
                texture2D(inputImageTexture3, vec2(texel.r, .16666)).r,
                texture2D(inputImageTexture3, vec2(texel.g, .5)).g,
                texture2D(inputImageTexture3, vec2(texel.b, .83333)).b);

    vec3 luma = vec3(.30, .59, .11);
    vec3 gradSample = texture2D(inputImageTexture4, vec2(dot(luma, texel), .5)).rgb;
    vec3 final = vec3(
                     texture2D(inputImageTexture5, vec2(gradSample.r, texel.r)).r,
                     texture2D(inputImageTexture5, vec2(gradSample.g, texel.g)).g,
                     texture2D(inputImageTexture5, vec2(gradSample.b, texel.b)).b
                 );

    vec3 metal = texture2D(inputImageTexture6, vv2_Texcoord).rgb;
    vec3 metaled = vec3(
                       texture2D(inputImageTexture5, vec2(metal.r, texel.r)).r,
                       texture2D(inputImageTexture5, vec2(metal.g, texel.g)).g,
                       texture2D(inputImageTexture5, vec2(metal.b, texel.b)).b
                   );

    metaled.rgb = mix(originColor.rgb, metaled.rgb, strength);

    gl_FragColor = vec4(metaled, 1.0);
}
);

const char *XM_BFilter_getFragmentShader_hefe()
{
    return g_shader_hefe;
}


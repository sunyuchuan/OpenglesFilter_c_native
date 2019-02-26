/*
* Created by sunyc on 2018-8-24.
* Create Fragment shaders of fairytale effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_fairytale[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    varying   highp vec2 vv2_Texcoord_1;
    uniform   lowp  sampler2D us2_Sampler;
    uniform   lowp  sampler2D us2_Sampler_1; // lookup texture

    void main()
    {
        lowp vec4 textureColor = texture2D(us2_Sampler, vv2_Texcoord);
        mediump float blueColor = textureColor.b * 63.0;

        mediump vec2 quad1;
        quad1.y = floor(floor(blueColor) / 8.0);
        quad1.x = floor(blueColor) - (quad1.y * 8.0);

        mediump vec2 quad2;
        quad2.y = floor(ceil(blueColor) / 8.0);
        quad2.x = ceil(blueColor) - (quad2.y * 8.0);

        highp vec2 texPos1;
        texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
        texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);

        highp vec2 texPos2;
        texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
        texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);

        lowp vec4 newColor1 = texture2D(us2_Sampler_1, texPos1);
        lowp vec4 newColor2 = texture2D(us2_Sampler_1, texPos2);

        lowp vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
        gl_FragColor = vec4(newColor.rgb, textureColor.w);
    }
);

const char *XM_BFilter_getFragmentShader_fairytale()
{
    return g_shader_fairytale;
}


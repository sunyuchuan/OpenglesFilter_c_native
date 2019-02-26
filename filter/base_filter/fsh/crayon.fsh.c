/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of crayon effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_crayon[] = XM_GLES_STRING(
precision highp float;
varying highp vec2 vv2_Texcoord;
uniform lowp sampler2D us2_Sampler;

uniform vec2 singleStepOffset;
uniform float strength;

const highp vec3 W = vec3(0.299,0.587,0.114);
const mat3 rgb2yiqMatrix = mat3(
           0.299, 0.587, 0.114,
           0.596,-0.275,-0.321,
           0.212,-0.523, 0.311);
const mat3 yiq2rgbMatrix = mat3(
           1.0, 0.956, 0.621,
           1.0,-0.272,-1.703,
           1.0,-1.106, 0.0);

void main()
{
    vec4 oralColor = texture2D(us2_Sampler, vv2_Texcoord);

    vec3 maxValue = vec3(0.,0.,0.);

    for(int i = -2; i<=2; i++)
    {
        for(int j = -2; j<=2; j++)
        {
            vec4 tempColor = texture2D(us2_Sampler, vv2_Texcoord+singleStepOffset*vec2(i,j));
            maxValue.r = max(maxValue.r,tempColor.r);
            maxValue.g = max(maxValue.g,tempColor.g);
            maxValue.b = max(maxValue.b,tempColor.b);
        }
    }

    vec3 textureColor = oralColor.rgb / maxValue;

    float gray = dot(textureColor, W);
    float k = 0.223529;
    float alpha = min(gray,k)/k;

    textureColor = textureColor * alpha + (1.-alpha)*oralColor.rgb;

    vec3 yiqColor = textureColor * rgb2yiqMatrix;

    yiqColor.r = max(0.0,min(1.0,pow(gray,strength)));

    textureColor = yiqColor * yiq2rgbMatrix;

    gl_FragColor = vec4(textureColor, oralColor.w);
}
);

const char *XM_BFilter_getFragmentShader_crayon()
{
    return g_shader_crayon;
}


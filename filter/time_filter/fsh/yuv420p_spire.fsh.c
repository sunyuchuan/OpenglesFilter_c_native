/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of swirl effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_spire[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    uniform         mat3 um3_ColorConversion;
    uniform   lowp  sampler2D us2_SamplerX;
    uniform   lowp  sampler2D us2_SamplerY;
    uniform   lowp  sampler2D us2_SamplerZ;

    uniform highp vec2 center;
    uniform highp float radius;
    uniform highp float angle;

    void main()
    {
        mediump vec3 yuv;
        lowp    vec3 rgb;

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

        //gl_FragColor = texture2D(inputImageTexture, textureCoordinateToUse );
        yuv.x = (texture2D(us2_SamplerX, textureCoordinateToUse).r - (16.0 / 255.0));
        yuv.y = (texture2D(us2_SamplerY, textureCoordinateToUse).r - 0.5);
        yuv.z = (texture2D(us2_SamplerZ, textureCoordinateToUse).r - 0.5);
        rgb = um3_ColorConversion * yuv;
        gl_FragColor = vec4(rgb, 1);
    }
);

const char *XM_TFilter_yuv420p_getFragmentShader_spire()
{
    return g_shader_spire;
}


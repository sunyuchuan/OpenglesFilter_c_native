/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of hue effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_hue[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    uniform         mat3 um3_ColorConversion;
    uniform   lowp  sampler2D us2_SamplerX;
    uniform   lowp  sampler2D us2_SamplerY;
    uniform   lowp  sampler2D us2_SamplerZ;

    uniform mediump float hueAdjust;
    const highp vec4 kRGBToYPrime = vec4 (0.299, 0.587, 0.114, 0.0);
    const highp vec4 kRGBToI = vec4 (0.595716, -0.274453, -0.321263, 0.0);
    const highp vec4 kRGBToQ = vec4 (0.211456, -0.522591, 0.31135, 0.0);

    const highp vec4 kYIQToR = vec4 (1.0, 0.9563, 0.6210, 0.0);
    const highp vec4 kYIQToG = vec4 (1.0, -0.2721, -0.6474, 0.0);
    const highp vec4 kYIQToB = vec4 (1.0, -1.1070, 1.7046, 0.0);

    void main ()
    {
        mediump vec3 yuv;
        lowp	vec3 rgb;

        yuv.x = (texture2D(us2_SamplerX, vv2_Texcoord).r - (16.0 / 255.0));
        yuv.y = (texture2D(us2_SamplerY, vv2_Texcoord).r - 0.5);
        yuv.z = (texture2D(us2_SamplerZ, vv2_Texcoord).r - 0.5);
        rgb = um3_ColorConversion * yuv;

        // Sample the input pixel
        highp vec4 color = vec4(rgb, 1);

        // Convert to YIQ
        highp float YPrime = dot (color, kRGBToYPrime);
        highp float I = dot (color, kRGBToI);
        highp float Q = dot (color, kRGBToQ);

        // Calculate the hue and chroma
        highp float hue = atan (Q, I);
        highp float chroma = sqrt (I * I + Q * Q);

        // Make the user's adjustments
        hue += (-hueAdjust); //why negative rotation?

        // Convert back to YIQ
        Q = chroma * sin (hue);
        I = chroma * cos (hue);

        // Convert back to RGB
        highp vec4 yIQ = vec4 (YPrime, I, Q, 0.0);
        color.r = dot (yIQ, kYIQToR);
        color.g = dot (yIQ, kYIQToG);
        color.b = dot (yIQ, kYIQToB);

        // Save the result
        gl_FragColor = color;
    }
);

const char *XM_TFilter_yuv420p_getFragmentShader_hue()
{
    return g_shader_hue;
}


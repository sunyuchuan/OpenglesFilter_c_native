/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of vivgnette effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_vivgnette[] = XM_GLES_STRING(
    precision highp float;
    varying highp vec2 vv2_Texcoord;
    uniform lowp sampler2D us2_Sampler;

    uniform lowp vec2 vignetteCenter;
    uniform lowp vec3 vignetteColor;
    uniform highp float vignetteStart;
    uniform highp float vignetteEnd;

    void main()
    {
        /*
        lowp vec3 rgb = texture2D(us2_Sampler, vv2_Texcoord).rgb;
        lowp float d = distance(vv2_Texcoord, vec2(0.5,0.5));
        rgb *= (1.0 - smoothstep(vignetteStart, vignetteEnd, d));
        gl_FragColor = vec4(vec3(rgb),1.0);
        */

        lowp vec3 rgb = texture2D(us2_Sampler, vv2_Texcoord).rgb;
        lowp float d = distance(vv2_Texcoord, vec2(vignetteCenter.x, vignetteCenter.y));
        lowp float percent = smoothstep(vignetteStart, vignetteEnd, d);
        gl_FragColor = vec4(mix(rgb.x, vignetteColor.x, percent), mix(rgb.y, vignetteColor.y, percent), mix(rgb.z, vignetteColor.z, percent), 1.0);
    }
);

const char *XM_BFilter_getFragmentShader_vivgnette()
{
    return g_shader_vivgnette;
}


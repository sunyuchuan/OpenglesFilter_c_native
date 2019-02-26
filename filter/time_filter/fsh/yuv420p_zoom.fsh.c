/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of magnifying-glass effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_zoom[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    varying   highp vec4 g_position;
    uniform   vec3 vv3_dot;
    uniform   float uXY;
    uniform         mat3 um3_ColorConversion;
    uniform   lowp  sampler2D us2_SamplerX;
    uniform   lowp  sampler2D us2_SamplerY;
    uniform   lowp  sampler2D us2_SamplerZ;

    void main()
    {
        mediump vec3 yuv;
        lowp    vec3 rgb;
        highp vec2 zoom = vec2(vv2_Texcoord.x/2.0 + 0.25, vv2_Texcoord.y/2.0 + 0.25);
        float dis = distance(vec2(g_position.x * uXY, g_position.y), vec2(vv3_dot.x , vv3_dot.y));
        //float ratio = sqrt(dis / vv3_dot.z);
        //float x1 = vv3_dot.x - (vv3_dot.x - g_position.x) * ratio;
        //float y1 = vv3_dot.y - (vv3_dot.y - g_position.y) * ratio;
        //highp vec2 zoom = vec2(x1, 1.0-y1);

        if(dis < vv3_dot.z) {
            yuv.x = (texture2D(us2_SamplerX, zoom).r - (16.0 / 255.0));
            yuv.y = (texture2D(us2_SamplerY, zoom).r - 0.5);
            yuv.z = (texture2D(us2_SamplerZ, zoom).r - 0.5);
        } else {
            yuv.x = (texture2D(us2_SamplerX, vv2_Texcoord).r - (16.0 / 255.0));
            yuv.y = (texture2D(us2_SamplerY, vv2_Texcoord).r - 0.5);
            yuv.z = (texture2D(us2_SamplerZ, vv2_Texcoord).r - 0.5);
        }
        rgb = um3_ColorConversion * yuv;
        gl_FragColor = vec4(rgb, 1);
    }
);

const char *XM_TFilter_yuv420p_getFragmentShader_zoom()
{
    return g_shader_zoom;
}


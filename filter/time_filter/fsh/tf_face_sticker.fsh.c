/*
* Created by sunyc on 2018-10-8.
* Create Fragment shaders of face sticker effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_face_sticker[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    varying   highp vec4 g_position;
    uniform         mat3 um3_ColorConversion;
    uniform   lowp  sampler2D us2_SamplerX;
    uniform   lowp  sampler2D us2_SamplerY;
    uniform   lowp  sampler2D us2_SamplerZ;
    uniform   lowp  sampler2D inputImageTexture2;
    uniform   highp vec2 leftTop;
    uniform   highp vec2 rightBottom;
    uniform   highp float offScreen;

    void main()
    {
        lowp vec4 rgba1;
        lowp vec4 rgba2;
        lowp vec2 vv2_Texcoord2use;
        mediump vec3 yuv;
        lowp vec3 rgb;

        yuv.x = (texture2D(us2_SamplerX, vv2_Texcoord).r - (16.0 / 255.0));
        yuv.y = (texture2D(us2_SamplerY, vv2_Texcoord).r - 0.5);
        yuv.z = (texture2D(us2_SamplerZ, vv2_Texcoord).r - 0.5);
        rgb = um3_ColorConversion * yuv;
        rgba1 = vec4(rgb, 1.0);

        if (g_position.x >= leftTop.x && g_position.y >= leftTop.y
            && g_position.x <= rightBottom.x && g_position.y <= rightBottom.y) {

            if(offScreen > 0.0) {
                vv2_Texcoord2use = vec2(1.0 - (g_position.x - leftTop.x) / (rightBottom.x - leftTop.x),
                    (g_position.y - leftTop.y) / (rightBottom.y - leftTop.y));
            } else {
                vv2_Texcoord2use = vec2((g_position.x - leftTop.x) / (rightBottom.x - leftTop.x),
                    1.0 - (g_position.y - leftTop.y) / (rightBottom.y - leftTop.y));
            }
            rgba2 = texture2D(inputImageTexture2, vv2_Texcoord2use);
            gl_FragColor = rgba2 * rgba2.a + rgba1 * (1.0 - rgba2.a);
        } else {
            gl_FragColor = rgba1;
        }
    }
);

const char *XM_TFilter_yuv420p_getFragmentShader_face_sticker()
{
    return g_shader_face_sticker;
}


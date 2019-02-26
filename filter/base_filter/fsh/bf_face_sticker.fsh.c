/*
* Created by sunyc on 2018-10-8.
* Create Fragment shaders of face sticker effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_face_sticker[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    varying   highp vec4 g_position;
    uniform   lowp  sampler2D us2_Sampler;
    uniform   highp  sampler2D inputImageTexture2;
    uniform   highp vec2 leftTop;
    uniform   highp vec2 rightBottom;
    uniform   highp float offScreen;

    void main()
    {
        highp vec4 rgba1;
        highp vec4 rgba2;
        lowp vec2 vv2_Texcoord2use;

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
            rgba1 = texture2D(us2_Sampler, vv2_Texcoord);
            gl_FragColor = rgba2 * rgba2.a + rgba1 * (1.0 - rgba2.a);
        } else {
            gl_FragColor = texture2D(us2_Sampler, vv2_Texcoord);
        }
    }
);

const char *XM_BFilter_getFragmentShader_face_sticker()
{
    return g_shader_face_sticker;
}


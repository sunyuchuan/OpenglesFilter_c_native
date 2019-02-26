/*
 * Created by sunyc on 2018-8-27.
 * Create Vertex shaders of fairytale effects
 */

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_fairytale[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    varying   highp vec2 vv2_Texcoord_1;
    attribute highp vec4 av4_Position;
    attribute highp vec2 av2_Texcoord;
    attribute highp vec2 av2_Texcoord_1;
    uniform         mat4 um4_ModelViewProjection;

    void main()
    {
        gl_Position  = um4_ModelViewProjection * av4_Position;
        vv2_Texcoord = av2_Texcoord.xy;
        vv2_Texcoord_1 = av2_Texcoord_1.xy;
    }
);

const char *XM_GLES2_BaseFilter_getVertexShader_fairytale()
{
    return g_shader_fairytale;
}


/*
 * Created by sunyc on 2018-6-25.
 * Create Vertex shaders of retro effects
 */

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_retro[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    attribute highp vec4 av4_Position;
    attribute highp vec2 av2_Texcoord;
    uniform         mat4 um4_ModelViewProjection;

    void main()
    {
        gl_Position  = um4_ModelViewProjection * av4_Position;
        vv2_Texcoord = av2_Texcoord.xy;
    }
);

const char *XM_GLES2_BaseFilter_getVertexShader_retro()
{
    return g_shader_retro;
}


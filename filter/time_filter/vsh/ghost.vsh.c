/*
 * Created by sunyc on 2018-6-25.
 * Create Vertex shaders of soul effects
 */

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_ghost[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    varying   highp vec2 vv2_Texcoord_ghost;
    attribute highp vec4 av4_Position;
    attribute highp vec2 av2_Texcoord;
    attribute highp vec2 av2_Texcoord_ghost;
    uniform         mat4 um4_ModelViewProjection;

    void main()
    {
        gl_Position  = um4_ModelViewProjection * av4_Position;
        vv2_Texcoord = av2_Texcoord.xy;
        vv2_Texcoord_ghost = av2_Texcoord_ghost.xy;
    }
);

const char *XM_GLES2_TimeFilter_getVertexShader_ghost()
{
    return g_shader_ghost;
}


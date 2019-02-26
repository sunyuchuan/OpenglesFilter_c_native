/*
 * Created by sunyc on 2018-10-8.
 * Create Vertex shaders of face sticker effects.
 */

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_face_sticker[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    varying   highp vec4 g_position;
    attribute highp vec4 av4_Position;
    attribute highp vec2 av2_Texcoord;
    uniform         mat4 um4_ModelViewProjection;

    void main()
    {
        gl_Position  = um4_ModelViewProjection * av4_Position;
        g_position = gl_Position;
        vv2_Texcoord = av2_Texcoord.xy;
    }
);

const char *XM_GLES2_getVertexShader_face_sticker()
{
    return g_shader_face_sticker;
}


/*
 * Created by sunyc on 2018-6-25.
 * Create Vertex shaders of magnifying-glass effects
 */

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_zoom[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    varying   highp vec4 g_position;
    attribute highp vec4 av4_Position;
    attribute highp vec2 av2_Texcoord;
    uniform         mat4 um4_ModelViewProjection;

    void main()
    {
        gl_Position  = um4_ModelViewProjection * av4_Position;
        vv2_Texcoord = av2_Texcoord.xy;
        g_position = um4_ModelViewProjection * av4_Position;
    }
);

const char *XM_GLES2_TimeFilter_getVertexShader_zoom()
{
    return g_shader_zoom;
}


/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of inkwell effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_inkwell[] = XM_GLES_STRING(
        precision highp float;
        varying highp vec2 vv2_Texcoord;
        uniform lowp sampler2D us2_Sampler;

        uniform lowp sampler2D inputImageTexture2;

void main()
{
    vec3 texel = texture2D(us2_Sampler, vv2_Texcoord).rgb;
    texel = vec3(dot(vec3(0.3, 0.6, 0.1), texel));
    texel = vec3(texture2D(inputImageTexture2, vec2(texel.r, .16666)).r);
    gl_FragColor = vec4(texel, 1.0);
}
);

const char *XM_BFilter_getFragmentShader_inkwell()
{
    return g_shader_inkwell;
}


/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of cross hatch effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_crosshatch[] = XM_GLES_STRING(
    precision highp float;
    varying highp vec2 vv2_Texcoord;
    uniform lowp sampler2D us2_Sampler;

    uniform highp float crossHatchSpacing;
    uniform highp float lineWidth;
    const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);
    void main()
    {
        highp float luminance = dot(texture2D(us2_Sampler, vv2_Texcoord).rgb, W);
        lowp vec4 colorToDisplay = vec4(1.0, 1.0, 1.0, 1.0);

        if (luminance < 1.00)
        {
            if (mod(vv2_Texcoord.x + vv2_Texcoord.y, crossHatchSpacing) <= lineWidth)
            {
                colorToDisplay = vec4(0.0, 0.0, 0.0, 1.0);
            }
        }

        if (luminance < 0.75)
        {
            if (mod(vv2_Texcoord.x - vv2_Texcoord.y, crossHatchSpacing) <= lineWidth)
            {
                colorToDisplay = vec4(0.0, 0.0, 0.0, 1.0);
            }
        }

        if (luminance < 0.50)
        {
            if (mod(vv2_Texcoord.x + vv2_Texcoord.y - (crossHatchSpacing / 2.0), crossHatchSpacing) <= lineWidth)
            {
                colorToDisplay = vec4(0.0, 0.0, 0.0, 1.0);
            }
        }

        if (luminance < 0.3)
        {
            if (mod(vv2_Texcoord.x - vv2_Texcoord.y - (crossHatchSpacing / 2.0), crossHatchSpacing) <= lineWidth)
            {
                colorToDisplay = vec4(0.0, 0.0, 0.0, 1.0);
            }
        }

        gl_FragColor = colorToDisplay;
    }
);

const char *XM_BFilter_getFragmentShader_crosshatch()
{
    return g_shader_crosshatch;
}


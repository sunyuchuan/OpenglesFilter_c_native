/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of calm effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_calm[] = XM_GLES_STRING(
precision highp float;
varying highp vec2 vv2_Texcoord;
uniform lowp sampler2D us2_Sampler;

uniform lowp sampler2D grey1Frame;
uniform lowp sampler2D grey2Frame;
uniform lowp sampler2D curve;

const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);

void main()
{
    lowp float satura = 0.5;
    float GreyVal;
    lowp vec4 textureColor;
    lowp vec4 textureColorRes;

    highp float redCurveValue;
    highp float greenCurveValue;
    highp float blueCurveValue;

    vec4 grey1Color;
    vec4 grey2Color;

    float xCoordinate = vv2_Texcoord.x;
    float yCoordinate = vv2_Texcoord.y;

    textureColor = texture2D( us2_Sampler, vec2(xCoordinate, yCoordinate));
    textureColorRes = textureColor;

    grey1Color = texture2D(grey1Frame, vec2(xCoordinate, yCoordinate));
    grey2Color = texture2D(grey2Frame, vec2(xCoordinate, yCoordinate));

    // step 1. saturation
    lowp float luminance = dot(textureColor.rgb, luminanceWeighting);
    lowp vec3 greyScaleColor = vec3(luminance);

    textureColor = vec4(mix(greyScaleColor, textureColor.rgb, satura), textureColor.w);

    // step 2. level, blur curve, rgb curve
    redCurveValue = texture2D(curve, vec2(textureColor.r, 0.0)).r;
    redCurveValue = texture2D(curve, vec2(redCurveValue, 1.0/2.0)).r;

    greenCurveValue = texture2D(curve, vec2(textureColor.g, 0.0)).g;
    greenCurveValue = texture2D(curve, vec2(greenCurveValue, 1.0/2.0)).g;

    blueCurveValue = texture2D(curve, vec2(textureColor.b, 0.0)).b;
    blueCurveValue = texture2D(curve, vec2(blueCurveValue, 1.0/2.0)).b;
    blueCurveValue = texture2D(curve, vec2(blueCurveValue, 1.0/2.0)).g;

    lowp vec4 base = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0);

    redCurveValue = texture2D(curve, vec2(redCurveValue, 1.0)).r;
    greenCurveValue = texture2D(curve, vec2(greenCurveValue, 1.0)).r;
    blueCurveValue = texture2D(curve, vec2(blueCurveValue, 1.0)).r;
    lowp vec4 overlayer = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0);
    //gl_FragColor = base * (1.0 - grey1Color.r) + overlayer * grey1Color.r;
    base = (base - overlayer) * (1.0 - grey1Color.r) + overlayer;

    redCurveValue = texture2D(curve, vec2(base.r, 1.0)).g;
    greenCurveValue = texture2D(curve, vec2(base.g, 1.0)).g;
    blueCurveValue = texture2D(curve, vec2(base.b, 1.0)).g;
    overlayer = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0);

    textureColor = (base - overlayer) * (1.0 - grey2Color.r) + overlayer;
    //base * (grey2Color.r) + overlayer * (1.0 - grey2Color.r);

    gl_FragColor = vec4(textureColor.r, textureColor.g, textureColor.b, 1.0);
}
);

const char *XM_BFilter_getFragmentShader_calm()
{
    return g_shader_calm;
}


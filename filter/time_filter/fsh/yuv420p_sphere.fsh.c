/*
* Created by sunyc on 2018-8-23.
* Create Fragment shaders of Sphere Refraction effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_sphere[] = XM_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
    uniform         mat3 um3_ColorConversion;
    uniform   lowp  sampler2D us2_SamplerX;
    uniform   lowp  sampler2D us2_SamplerY;
    uniform   lowp  sampler2D us2_SamplerZ;
    uniform highp vec2 center;
    uniform highp float radius;
    uniform highp float aspectRatio;
    uniform highp float refractiveIndex;

    void main()
    {
        mediump vec3 yuv;
        lowp    vec3 rgb;

        highp vec2 vv2_Texcoord_sphere = vec2(vv2_Texcoord.x, (vv2_Texcoord.y * aspectRatio + 0.5 - 0.5 * aspectRatio));
        highp float dis = distance(center, vv2_Texcoord_sphere);
        lowp float checkForPresenceWithinSphere = step(dis, radius);
        dis = dis / radius;
        highp float normalizedDepth = radius * sqrt(1.0 - dis * dis);
        highp vec3 sphereNormal = normalize(vec3(vv2_Texcoord_sphere - center, normalizedDepth));
        highp vec3 refractedVector = refract(vec3(0.0, 0.0, -1.0), sphereNormal, refractiveIndex);

        yuv.x = (texture2D(us2_SamplerX, (refractedVector.xy + 1.0) * 0.5).r - (16.0 / 255.0));
        yuv.y = (texture2D(us2_SamplerY, (refractedVector.xy + 1.0) * 0.5).r - 0.5);
        yuv.z = (texture2D(us2_SamplerZ, (refractedVector.xy + 1.0) * 0.5).r - 0.5);

        rgb = um3_ColorConversion * yuv;
        gl_FragColor = vec4(rgb, 1) * checkForPresenceWithinSphere;
    }
);

const char *XM_TFilter_yuv420p_getFragmentShader_sphere()
{
    return g_shader_sphere;
}


/*
* Created by sunyc on 2018-6-25.
* Create Fragment shaders of glass sphere effects
*/

#include "ijksdl/gles2_xm/internal.h"

static const char g_shader_glass_sphere[] = XM_GLES_STRING(
    precision highp float;
    varying highp vec2 vv2_Texcoord;
    uniform lowp sampler2D us2_Sampler;

    uniform highp vec2 center;
    uniform highp float radius;
    uniform highp float aspectRatio;
    uniform highp float refractiveIndex;
    // uniform vec3 lightPosition;
    const highp vec3 lightPosition = vec3(-0.5, 0.5, 1.0);
    const highp vec3 ambientLightPosition = vec3(0.0, 0.0, 1.0);

    void main()
    {
        highp vec2 textureCoordinateToUse = vec2(vv2_Texcoord.x, (vv2_Texcoord.y * aspectRatio + 0.5 - 0.5 * aspectRatio));
        highp float distanceFromCenter = distance(center, textureCoordinateToUse);
        lowp float checkForPresenceWithinSphere = step(distanceFromCenter, radius);

        distanceFromCenter = distanceFromCenter / radius;

        highp float normalizedDepth = radius * sqrt(1.0 - distanceFromCenter * distanceFromCenter);
        highp vec3 sphereNormal = normalize(vec3(textureCoordinateToUse - center, normalizedDepth));

        highp vec3 refractedVector = 2.0 * refract(vec3(0.0, 0.0, -1.0), sphereNormal, refractiveIndex);
        refractedVector.xy = -refractedVector.xy;

        highp vec3 finalSphereColor = texture2D(us2_Sampler, (refractedVector.xy + 1.0) * 0.5).rgb;

        // Grazing angle lighting
        highp float lightingIntensity = 2.5 * (1.0 - pow(clamp(dot(ambientLightPosition, sphereNormal), 0.0, 1.0), 0.25));
        finalSphereColor += lightingIntensity;

        // Specular lighting
        lightingIntensity  = clamp(dot(normalize(lightPosition), sphereNormal), 0.0, 1.0);
        lightingIntensity  = pow(lightingIntensity, 15.0);
        finalSphereColor += vec3(0.8, 0.8, 0.8) * lightingIntensity;

        gl_FragColor = vec4(finalSphereColor, 1.0) * checkForPresenceWithinSphere;
    }
);

const char *XM_BFilter_getFragmentShader_glass_sphere()
{
    return g_shader_glass_sphere;
}


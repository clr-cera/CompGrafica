#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 vertexColor;
in vec3 worldPosition;
in vec3 normal;

uniform sampler2D aTexture;
uniform float ambientLight;
uniform vec3 ambientColor;
uniform vec3 cameraPos;
uniform bool flipNormals;
uniform float specularStrength;
uniform float diffuseStrength;
uniform float objectDiffuse;
uniform float objectSpecular;

#define SHININESS 32.0

struct PointLight {
    vec3 position;
    vec3 color;
};
#define MAX_LIGHTS 10
uniform PointLight pointLights[MAX_LIGHTS];
uniform int currentLightCount;

vec3 calculateDiffuseLight(vec3 surfaceNormal);
vec3 calculateSpecularLight(vec3 surfaceNormal);

void main()
{
    vec3 finalNormal = normal;
    if (!gl_FrontFacing) {
        finalNormal = -finalNormal;
    }
    if (flipNormals) {
        finalNormal = -finalNormal;
    }

    vec3 lightCoeff = vec3(0.0);
    lightCoeff += ambientLight * ambientColor;
    lightCoeff += calculateDiffuseLight(finalNormal);
    lightCoeff += calculateSpecularLight(finalNormal);
    FragColor = vec4(min(lightCoeff, 1.0), 1.0) * texture(aTexture, texCoord);
}

vec3 calculateDiffuseLight(vec3 surfaceNormal)
{
    vec3 lightCoeff = vec3(0.0);
    for (int i = 0; i < currentLightCount; i++) {
        vec3 lightDir = normalize(pointLights[i].position - worldPosition);
        float diff = max(dot(surfaceNormal, lightDir), 0.0);
        lightCoeff += diff * pointLights[i].color;
    }
    return lightCoeff * diffuseStrength * objectDiffuse;
}


vec3 calculateSpecularLight(vec3 surfaceNormal) {
    vec3 specularCoeff  = vec3(0.0);
    for (int i = 0; i < currentLightCount; i++) {
        vec3 lightDir = normalize(pointLights[i].position - worldPosition);
        vec3 reflectDir = reflect(-lightDir, surfaceNormal);
        vec3 viewDir = normalize(cameraPos - worldPosition);
        specularCoeff += pow(max(dot(viewDir, reflectDir), 0.0), SHININESS) * specularStrength * objectSpecular * pointLights[i].color;
    }
    return specularCoeff;
}
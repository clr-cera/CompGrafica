#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 vertexColor;
in vec3 worldPosition;
in vec3 normal;

uniform sampler2D aTexture;
uniform float ambientLight;
uniform vec3 ambientColor;


struct PointLight {
    vec3 position;
    vec3 color;
};
#define MAX_LIGHTS 10
uniform PointLight pointLights[MAX_LIGHTS];
uniform int currentLightCount;

vec3 calculateDiffuseLight();

void main()
{
    vec3 lightCoeff = vec3(0.0);
    lightCoeff += ambientLight * ambientColor;
    lightCoeff += calculateDiffuseLight();
    FragColor = vec4(min(lightCoeff, 1.0), 1.0) * texture(aTexture, texCoord);
}

vec3 calculateDiffuseLight()
{
    vec3 lightCoeff = vec3(0.0);
    for (int i = 0; i < currentLightCount; i++) {
        vec3 lightDir = normalize(pointLights[i].position - worldPosition);
        float diff = max(dot(normal, lightDir), 0.0);
        lightCoeff += diff * pointLights[i].color;
    }
    return lightCoeff;
}
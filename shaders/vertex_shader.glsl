#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec3 vertexColor;
out vec2 texCoord;
out vec3 normal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 NormalMatrix;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = aColor;
    texCoord = aTexCoord;
    normal = normalize(NormalMatrix * aNormal);
}
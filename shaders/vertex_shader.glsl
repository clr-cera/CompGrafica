#version 330 core
in vec3 aPos;
in vec3 aColor;

out vec3 vertexColor;

uniform mat4 transform;

void main()
{
    gl_Position = mat_transformation * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = aColor;
}
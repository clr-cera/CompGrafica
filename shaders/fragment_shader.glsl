#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 vertexColor;

uniform sampler2D aTexture;
uniform float ambientLight;

void main()
{
    FragColor = ambientLight * texture(aTexture, texCoord);
}
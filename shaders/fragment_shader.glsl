#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 vertexColor;
in vec3 normal;
uniform sampler2D aTexture;
uniform float ambientLight;
uniform vec3 ambientColor;

void main()
{
    FragColor = vec4(ambientLight * ambientColor, 1.0) * texture(aTexture, texCoord);
}
#version 330 core
out vec4 FragColor;

uniform sampler2D texture0;

in vec2 TexCoords;

void main()
{
    vec4 texColor = texture(texture0, TexCoords);
    FragColor = texColor;
}

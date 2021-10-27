#version 330 core
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;


void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

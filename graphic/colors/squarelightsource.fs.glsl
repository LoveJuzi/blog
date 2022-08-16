#version 330 core
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightColor;

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;


void main()
{
    FragColor = vec4(lightColor, 1.0f);
}

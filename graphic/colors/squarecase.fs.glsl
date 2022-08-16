#version 330 core
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;


void main()
{
    FragColor = vec4(lightColor * objectColor, 1.0f);
}

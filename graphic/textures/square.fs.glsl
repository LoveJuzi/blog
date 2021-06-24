#version 330 core
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;


void main()
{
    FragColor = mix(texture2D(texture1, ourTexCoord), texture2D(texture2, ourTexCoord), 0.2);
}

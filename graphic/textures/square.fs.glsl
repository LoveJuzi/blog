#version 330 core
uniform sampler2D ourTexture;

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;


void main()
{
    FragColor = texture2D(ourTexture, ourTexCoord);
}

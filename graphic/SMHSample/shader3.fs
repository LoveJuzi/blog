#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord0;
in vec2 TexCoord1;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{      
    FragColor = mix(texture(texture0, TexCoord0), texture(texture1, TexCoord1), 0.2);
}

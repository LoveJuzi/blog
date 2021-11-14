#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;

uniform sampler2D texture1;

in  vec2 vTexCoord;
out vec4 FragColor;

void main() {
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    FragColor = texture(texture1, vTexCoord);
}

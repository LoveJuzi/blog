#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(lightColor, 1.0f);
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

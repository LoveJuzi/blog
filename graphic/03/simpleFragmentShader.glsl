#version 330 core

out vec4 FragColor;

in vec3 objColor;

uniform vec3 lightColor;

void main() {
    FragColor = vec4(lightColor * objColor, 1.0f);
}

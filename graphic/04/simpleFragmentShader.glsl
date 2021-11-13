#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;

out vec4 FragColor;
in  vec3 objColor;

void main() {
    FragColor = vec4(lightColor * objColor, 1.0f);
}

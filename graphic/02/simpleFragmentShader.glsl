#version 330 core

out vec4 FragColor;


void main() {
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 toyColor = vec3(1.0f, 0.5f, 0.31f);
    vec3 showFcolor = lightColor * toyColor;
    FragColor = vec4(showFcolor.xyz, 1.0f);
}

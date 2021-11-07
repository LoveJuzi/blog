#version 330 core

out vec4 FragColor;

in vec3 myColor;

void main() {
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 toyColor = vec3(1.0f, 0.5f, 0.31f);
    vec3 showFcolor = lightColor * toyColor;
    FragColor = vec4(myColor, 1.0f);
    // FragColor = vec4(showFcolor, 1.0f);
}

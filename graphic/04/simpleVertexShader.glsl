#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

layout(location = 0) in  vec3 aPos;
layout(location = 1) in  vec3 aColor;
layout(location = 2) in  vec3 aNormal;
                     out vec3 vColor;
                     out vec3 vNormal;
                     out vec3 fragPos;

void main() { 
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vColor = aColor;
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    fragPos = vec3(model * vec4(aPos, 1.0));
};

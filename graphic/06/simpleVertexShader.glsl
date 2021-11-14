#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

layout(location = 0) in  vec3 aPos;
layout(location = 1) in  vec3 aColor;
layout(location = 2) in  vec3 aNormal;
layout(location = 3) in  vec2 aTexCoords;
                     out vec3 vColor;
                     out vec3 vNormal;
                     out vec3 vFragPos;
                     out vec2 vTexCoords;

void main() { 
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vColor = aColor;
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vFragPos = vec3(model * vec4(aPos, 1.0));
    vTexCoords = aTexCoords;
};

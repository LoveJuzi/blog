#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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
uniform vec3 lightPos;
uniform vec3 viewPos;

in  vec3 vColor;
in  vec3 vNormal;
in  vec3 fragPos;
out vec4 FragColor;

void main() {
    vec3 ambient = lightColor * material.ambient;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * material.diffuse;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * lightColor * material.specular;

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}

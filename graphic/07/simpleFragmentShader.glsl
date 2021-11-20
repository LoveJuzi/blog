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

in  vec3 vColor;
in  vec3 vNormal;
in  vec3 vFragPos;
in  vec2 vTexCoords;
out vec4 FragColor;

void main() {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vTexCoords));

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.position - vFragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, vTexCoords));

    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, vTexCoords));

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}

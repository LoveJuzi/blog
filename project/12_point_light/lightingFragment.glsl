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

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main() {
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));

    float ambientStrength = 0.1;
    vec3 ambient = light.ambient * attenuation * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * attenuation *
                   vec3(texture(material.diffuse, TexCoords));

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * attenuation *
                    vec3(texture(material.specular, TexCoords));

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}

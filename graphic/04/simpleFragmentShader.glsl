#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;
uniform vec3 lightPos;

in  vec3 vColor;
in  vec3 vNormal;
in  vec3 fragPos;
out vec4 FragColor;

void main() {
    float ambientStrength = 0.1f; 
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    FragColor = vec4((ambient + diffuse) * vColor, 1.0f);
    // FragColor = vec4(ambient * vColor, 1.0f);
    // FragColor = vec4(vColor, 1.0f);
    // FragColor = vec4(lightColor, 1.0f);
    // FragColor = vec4(diffuse, 1.0f);
}

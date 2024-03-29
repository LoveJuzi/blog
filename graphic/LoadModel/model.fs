#version 330 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular3;
    sampler2D texture_diffuse4;
    sampler2D texture_specular4;
    sampler2D texture_diffuse5;
    sampler2D texture_specular5;
    float     shininess;
};

#define NR_POINT_LIGHTS 4

uniform vec3       viewPos;
uniform DirLight   dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight  spotLight;
uniform Material   material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0, 0.0, 0.0);

    // 开始绘制光照场景
    //
    // 1. 定向光
    //result += CalcDirLight(dirLight, normal, viewDir);

    // 2. 点光源
    for (int i=0; i<NR_POINT_LIGHTS; ++i) {
        //result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
    }

    // 3. 聚光
    //result += CalcSpotLight(spotLight, normal, FragPos, viewDir);

    //FragColor = vec4(result, 1.0);

    FragColor = texture(material.texture_diffuse1, TexCoords);
    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 objDiffColor = vec3(texture(material.diffuse, TexCoords));
    vec3 objSpecColor = vec3(texture(material.specular, TexCoords));

    float diff = max(dot(lightDir, normal), 0);

    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * objDiffColor;
    vec3 diffuse = light.diffuse * diff * objDiffColor;
    vec3 specular = light.specular * spec * objSpecColor;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 objDiffColor = vec3(texture(material.diffuse, TexCoords));
    vec3 objSpecColor = vec3(texture(material.specular, TexCoords));

    float diff =  max(dot(lightDir, normal), 0.0);

    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * distance +
                             light.quadratic * distance * distance);

    vec3 ambient = light.ambient * objDiffColor;
    vec3 diffuse = light.diffuse * diff * objDiffColor;
    vec3 specular = light.specular * spec * objSpecColor;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 objDiffColor = vec3(texture(material.diffuse, TexCoords));
    vec3 objSpecColor = vec3(texture(material.specular, TexCoords));

    float diff = max(dot(lightDir, normal), 0.0);

    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * distance +
                             light.quadratic * distance * distance);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient  = light.ambient * objDiffColor;
    vec3 diffuse  = light.diffuse * diff * objDiffColor;
    vec3 specular = light.specular * spec * objSpecColor;

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

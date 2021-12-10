#version 330 core
struct Materail {
    sampler2D texture_diffuse1;
};

uniform Materail material;

out vec4 FragColor;

in vec2 TexCoords;

void main() {    
    // FragColor = texture(material.texture_diffuse1, TexCoords);
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

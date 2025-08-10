#version 330 core

struct Material {
   sampler2D texture;
};

out vec4 FragColor;

uniform Material material;

in vec2 TexCoords;

void main() {
   FragColor = vec4(vec3(texture(material.texture, TexCoords)), 1.0);
}

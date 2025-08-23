#version 330 core

out vec4 FragColor;

uniform sampler2D atlas;

in vec2 TexCoords;

void main() {
   FragColor = vec4(vec3(texture(atlas, TexCoords)), 1.0);
}

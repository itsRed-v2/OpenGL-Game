#version 330 core

out vec4 FragColor;

uniform sampler2D image;

in vec2 TexCoords;

void main() {
    vec4 color = texture(image, TexCoords);
    FragColor = vec4(vec3(color), min(color.w, 0.75));
}

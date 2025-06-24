#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float time;

void main() {
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   gl_Position.x += 0.4 * sin(time * (5 + sin(aPos.y)));

   color = aColor;
}
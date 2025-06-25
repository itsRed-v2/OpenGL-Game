#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform vec2 windowSize;
uniform vec2 windowPos;

void main() {
   gl_Position.x = -1 + (aPos.x - windowPos.x) * 2 / windowSize.x;
   gl_Position.y = 1 - (aPos.y - windowPos.y) * 2 / windowSize.y;
   gl_Position.z = aPos.z;
   gl_Position.w = 1.0;

   color = aColor;
   texCoord = aTexCoord;
}
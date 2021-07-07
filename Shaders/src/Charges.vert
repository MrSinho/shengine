#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (location = 1) out vec4 fragPos;

void main() {

    fragPos = vec4(position, 1.0f);
    gl_Position = vec4(position, 1.0f);
}
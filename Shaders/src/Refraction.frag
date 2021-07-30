#version 450

layout (location = 0) in vec4 lineFragPos;
layout (location = 1) flat in vec3 lightProps;

layout (location = 0) out vec4 fragColor;

void main() {
    fragColor = lineFragPos;
}
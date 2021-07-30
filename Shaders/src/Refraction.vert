#version 450

layout (location = 0) in vec3  linePos;
layout (location = 1) in vec3  lightProps;

layout (location = 0) out vec4 lineFragPos;
layout (location = 1) out vec3 fraglightProps;

layout (binding = 0) uniform uniformBufferObject {
    float time;
} ubo;

void main() {
    lineFragPos = vec4(linePos, 1.0f);
    fraglightProps = lightProps;
    gl_Position = lineFragPos;
}
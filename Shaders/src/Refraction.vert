#version 450

layout (location = 0) in vec3 linepos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texcoords;

layout (location = 0) out vec4 lineFragPos;

layout (binding = 0) uniform uniformBufferObject {
    float time;
} ubo;

void main() {
    lineFragPos = vec4(linePos, 1.0f);
    gl_Position = lineFragPos;
}
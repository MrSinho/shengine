#version 450

layout (location = 0) in vec4 lineFragPos;

layout (location = 0) out vec4 fragPosition;

void main() {
    gl_Position = lineFragPos;
}
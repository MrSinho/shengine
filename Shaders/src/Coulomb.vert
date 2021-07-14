#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (location = 1) out vec4 fragPos;

layout (push_constant) uniform constants {
    mat4 projection;
    mat4 view;
} pushConstant;

void main() {

    fragPos = pushConstant.projection * vec4(position, 1.0f);
    gl_Position = pushConstant.projection * vec4(position, 1.0f);
}
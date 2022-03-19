#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvs;

layout (location = 0) out vec4 fragPosition;

layout (push_constant) uniform constants {
    mat4 projection;
    mat4 view;
} pconst;

layout (set = 0, binding = 1) uniform uniformBuffer { //binding = 1 because uniform is dynamic
    mat4 model;
} ubo;

void main() {
    fragPosition = ubo.model * vec4(position, 1.0f);
    gl_Position = pconst.projection * pconst.view * ubo.model * vec4(position, 1.0f);
}
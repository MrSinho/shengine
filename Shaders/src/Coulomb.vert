#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvs;

layout (location = 0) out mat4 MVP;
layout (location = 4) out vec4 fragPosition;

layout (push_constant) uniform constants {
    mat4 projection;
    mat4 view;
} pushConstants;

layout (set = 0, binding = 0) uniform uniformBuffer {
    mat4 model;
} ubo;

void main() {
    
    MVP = pushConstants.projection * pushConstants.view * ubo.model;
    fragPosition = MVP * vec4(position, 1.0f);
    gl_Position = fragPosition;
}
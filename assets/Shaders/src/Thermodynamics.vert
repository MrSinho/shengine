#version 460

layout (location = 0) in vec3 position;

layout (location = 0) out mat4 PV;
layout (location = 4) out vec4 fragPosition;

layout (push_constant) uniform constants {
    mat4 projection;
    mat4 view;
} pushConstants;

//UBO
layout (set = 0, binding = 0) uniform modelUbo {
    mat4 transform;
} ubo;

void main() {
    PV = pushConstants.projection * pushConstants.view;
    fragPosition = PV * ubo.transform * vec4(position, 1.0f);
    gl_Position = fragPosition;
}
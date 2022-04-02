#version 460

layout (location = 0) in vec3 position;

layout (location = 0) out vec4 fragment_position;
layout (location = 1) out uint vertex_index;

layout (push_constant) uniform pushConstant {
    mat4 projection;
    mat4 view;
} pconst;

//UBO
layout (set = 0, binding = 1) uniform modelUbo {
    mat4 transform;
} ubo;


void main() {
    vertex_index = gl_VertexIndex;
    fragment_position = pconst.projection * pconst.view * ubo.transform * vec4(position, 1.0f);
    gl_Position = fragment_position;
}
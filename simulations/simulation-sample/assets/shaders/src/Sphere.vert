#version 460
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_uv;

layout (location = 0) out vec4 fragment_position;

layout (push_constant) uniform _push_constant {
    mat4 projection;
    mat4 view;
} pconst;

layout (set = 0, binding = 1) uniform uniform_buffer {
    mat4 model;
} ubo;

void main() {

    fragment_position = ubo.model * vec4(vertex_position, 1.0f);

    gl_Position = pconst.projection * 
                    pconst.view * 
                   fragment_position;
;
}
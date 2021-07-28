#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 uv;
layout (location = 0) out vec4 fragPosition;

layout (push_constant) uniform constants {
	mat4 projection;
	mat4 view;
} pushConstants;

layout (set = 0, binding = 0) uniform uniformBuffer {
    mat4 model;
} ubo;


void main() {

  fragPosition = pushConstants.projection * pushConstants.view * ubo.model * vec4(position, 1.0f);
  gl_Position = fragPosition;
}
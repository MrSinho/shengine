#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 uv;

layout (push_constant) uniform constants {
	mat4 projection;
	mat4 view;
} pushConstants;

layout (location = 0) out vec4 color;

void main() {

  color = vec4(position, 1.0f);
  gl_Position = pushConstants.projection * pushConstants.view * vec4(position, 1.0f);
}
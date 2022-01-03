#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 uvs;

layout (location = 0) out vec4 fragPosition;
layout (location = 1) out vec4 rasterizedFragPosition;
//layout (location = 2) out vec3 fragNormal;
//layout (location = 1) out vec2 fragUV;

layout (push_constant) uniform constants {
	mat4 projection;
	mat4 view;
} pushConstants;

layout (set = 0, binding = 0) uniform uniformBuffer {
    mat4 model;
} ubo;


void main() {
  fragPosition = ubo.model * vec4(position, 1.0);
  rasterizedFragPosition = pushConstants.projection * pushConstants.view * ubo.model * vec4(position, 1..0);
  gl_Position = rasterizedFragPosition;
}
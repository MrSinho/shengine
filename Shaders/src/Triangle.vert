#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (push_constant) uniform constants {
	mat4 projection;
} pushConstants;

const vec4 colors[6] = vec4[6](
  vec4(  1.0f,   0.0f,   1.0f,  1.0f), 
	vec4(  1.0f,   0.5f,   0.0f,  1.0f), 
	vec4(  1.0f,   0.0f,   0.5f,  1.0f),
	vec4(  0.5f,   1.0f,   0.0f,  1.0f),
	vec4(  0.0f,   1.0f,   0.5f,  1.0f),
	vec4(  0.5f,   0.0f,   1.0f,  1.0f)
);

layout (location = 0) out vec4 color;

void main() {

  color = colors[gl_VertexIndex];
  gl_Position = pushConstants.projection * vec4(position, 1.0f);

}
#version 450

layout (location = 0) in vec2 position;

layout (location = 0) out vec4 frag_position;

layout (push_constant) uniform constants {
	mat4 projection;
	mat4 view;
} pconst;

void main() {
  
  gl_PointSize = 2.0f;

  frag_position = pconst.projection * pconst.view * vec4(position, 0.0, 1.0);
  gl_Position = frag_position;
}
#version 450

layout (location = 0) out vec4 frag_position;

vec4 canvas[6] = vec4[6](
	vec4(-1.0f,-1.0f, 0.5f, 1.0f),
	vec4( 1.0f, 1.0f, 0.5f, 1.0f),
	vec4(-1.0f, 1.0f, 0.5f, 1.0f),

	vec4( 1.0f, 1.0f, 0.5f, 1.0f),
	vec4(-1.0f,-1.0f, 0.5f, 1.0f),
	vec4( 1.0f,-1.0f, 0.5f, 1.0f)
);

void main() {
  
  gl_PointSize = 2.0f;

  frag_position = canvas[gl_VertexIndex % 6];
  gl_Position = frag_position;
}
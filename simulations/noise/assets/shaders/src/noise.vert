#version 450

layout (location = 0) out vec4 frag_position;

//layout (push_constant) uniform constants {
//	mat4 projection;
//	mat4 view;
//} pconst;

//layout (std140, set = 0, binding = 0) uniform uFractalProperties {
//    float zoom;
//} ufrac;

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

  frag_position = vec4(canvas[gl_VertexIndex % 6]);
  gl_Position = frag_position;
}
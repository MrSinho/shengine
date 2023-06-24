#version 460

//per vertex data
layout (location = 0) in vec2 position;

//output to fragment shader
layout (location = 0) out vec4 frag_position;



void main() {
  frag_position = vec4(position, 0.0, 1.0);
  gl_Position   = frag_position;
}
#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

const vec4 colors[3] = vec4[3](
  vec4(0.0f, 1.0f, 0.0f, 1.0f),
  vec4(1.0f, 0.0f, 0.0f, 1.0f),
  vec4(0.0f, 1.0f, 1.0f, 1.0f)
);

layout (location = 0) out vec4 color;

void main() {

  color = colors[gl_VertexIndex];
  gl_Position = vec4(position, 1.0f);

}
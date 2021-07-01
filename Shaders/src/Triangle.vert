#version 450

const vec4 positions[3] = vec4[3](
    vec4(1.0f, 1.0f, 0.0f, 1.0f),
    vec4(-1.0f, 1.0f, 0.0f, 1.0f),
    vec4(0.0f, -1.0f, 0.0f, 1.0f)
);

const vec4 colors[3] = vec4[3](
  vec4(0.0f, 1.0f, 0.0f, 1.0f),
  vec4(1.0f, 0.0f, 0.0f, 1.0f),
  vec4(0.0f, 1.0f, 1.0f, 1.0f)
);

layout (location = 1) out vec4 color;

void main() {

  color = colors[gl_VertexIndex];
  gl_Position = positions[gl_VertexIndex];

}
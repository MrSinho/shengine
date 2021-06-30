#version 450

const vec4 positions[3] = vec4[3](
    vec4(1.0f, 1.0f, 0.0f, 1.0f),
    vec4(-1.0f, 1.0f, 0.0f, 1.0f),
    vec4(0.0f, -1.0f, 0.0f, 1.0f)
);

void main() {

  gl_Position = positions[gl_VertexIndex];

}
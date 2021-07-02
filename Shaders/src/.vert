#version 450

const vec4 viewport[6] = vec4[6](
    vec4(1.0f, 1.0f, 0.0f, 1.0f),
    vec4(-1.0f, 1.0f, 0.0f, 1.0f),
    vec4(-1.0f, -1.0f, 0.0f, 1.0f),
    vec4(1.0f, 1.0f, 0.0f, 1.0f),
    vec4(1.0f, -1.0f, 0.0f, 1.0f),
    vec4(-1.0f, -1.0f, 0.0f, 1.0f)
);

layout (location = 1) out vec4 fragPos;

void main() {

    fragPos = viewport[gl_VertexIndex];
    gl_Position = vec4(fragPos);
}
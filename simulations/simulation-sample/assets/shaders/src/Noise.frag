#version 460

layout (location = 0) in vec4 fragment_position;
flat layout (location = 1) in uint vertex_index;

layout (location = 0) out vec4 fragment_color;

vec4 noise() {
    float value = float(vertex_index) - float(vertex_index) / 2.0f;

    vec4 color = vec4(length(fragment_position) * sqrt(value * 0.5f));
    color.w = 1.0f;
    return color;
}

void main() {

    fragment_color = vec4(noise());
}
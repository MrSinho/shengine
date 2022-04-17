#version 460


layout (location = 0) in vec4 fragment_position;
layout (location = 0) out vec4 fragment_color; 

layout (set = 1, binding = 0) uniform uniform_buffer {
    vec4    sphere_position;
    vec3    sphere_color;
    float   sphere_radius;
} ubo;

void main() {

    if (distance(fragment_position, ubo.sphere_position) > ubo.sphere_radius) {
        discard;
    }
    else {
        fragment_color = vec4(ubo.sphere_color, 1.0f);
    }

}
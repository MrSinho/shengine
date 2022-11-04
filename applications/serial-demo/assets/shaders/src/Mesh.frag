#version 460
flat layout (location = 0) in vec4 frag_position;
flat layout (location = 1) in vec4 frag_normal;

layout (location = 0) out vec4 frag_color;

struct sun_t {
    vec4 position;
    vec4 color;
    vec4 direction;
};

layout (set = 1, binding = 1) uniform uniformBuffer_1 { //binding = 1 because is dynamic
    vec4 base_color;
} ubo_1;

layout (set = 2, binding = 0) uniform uniformBuffer_2 { //binding = 0 because is not dynamic
    sun_t sun;
} ubo_2;

vec4 sun_intensity(float intensityCoefficient) {
    vec4 d = frag_position - ubo_2.sun.position;
    return vec4(intensityCoefficient * ubo_2.sun.color / (dot(d, d) + 0.01f));
}

void main(){

    float k = 0.01;
    frag_color = ubo_1.base_color * dot(normalize(frag_normal), normalize(ubo_2.sun.direction)) + sun_intensity(k);
}

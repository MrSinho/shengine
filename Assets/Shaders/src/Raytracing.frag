#version 450

layout (location = 0) in vec4 fragPosition;

layout (location = 0) out vec4 fragColor;
layout (location = 0) out vec4 rasterizedFragPosition;

struct Globals {
    vec2 resolution;
    vec3 cameraPosition;
    uint iterations;
    float maxDistance;
};

layout (set = 1, binding = 0) uniform uniformBuffer {
    Globals globals;

} ubo;

void main(){

    vec4 fragDir = rasterizedFragPosition - globals.cameraPosition;

    fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);   
}
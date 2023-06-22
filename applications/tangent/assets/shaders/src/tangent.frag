#version 450

layout (location = 0) in vec4 frag_position;
layout (location = 0) out vec4 fragColor;

layout (std140, set = 0, binding = 0) uniform _tangent_data {
    float x_0;
    float x_scale;
} tangent_data;

#define pi 3.1415926535

void main() {

    float x = tangent_data.x_scale * (tangent_data.x_0 + frag_position.x);

    if (
        x > -0.05f && 
        x < +0.05f 
        ) {
        fragColor = vec4(1.0f);
        return;
    }
    
    float remainder = x - (pi * floor(x/pi));

    if (
        remainder > -0.05f && 
        remainder < +0.05f 
        ) {
        fragColor = vec4(1.0f);
        return;
    }

    fragColor = vec4(
        normalize(
            vec3(
                tan(6*x),
                -tan(2*x),
                1.0f    
            )
        ),
        1.0f
    );

    return;
}
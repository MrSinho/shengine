#version 460
layout (location = 0) in vec4 fragPosition;
layout (location = 0) out vec4 fragColor;

struct ShPointLight {
    vec4 position;
    vec4 color;
};

layout (set = 1, binding = 1) uniform uniformBuffer_1 { //binding = 1 because is dynamic
    vec4 base_color;
} ubo_1;

layout (set = 2, binding = 0) uniform uniformBuffer_2 { //binding = 0 because is not dynamic
    ShPointLight point_lights[4];
} ubo_2;

vec4 shPointLightIntensity(float intensityCoefficient, uint chargeIdx) {
    float r = distance(fragPosition, ubo_2.point_lights[chargeIdx].position);
    return vec4(intensityCoefficient * ubo_2.point_lights[chargeIdx].color / (r*r));
}

void main(){

    float k = 1000.0;
    vec4 fieldIntensity = vec4(0.0);
    for (uint i = 0; i < 4; i++) {
        fieldIntensity += shPointLightIntensity(k, i);
    }
    fragColor = fieldIntensity + ubo_1.base_color;
}

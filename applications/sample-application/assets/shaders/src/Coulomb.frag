#version 460
layout (location = 0) in vec4 fragPosition;
layout (location = 0) out vec4 fragColor;

struct Charge {
    vec4 position;
    vec4 color;
};

layout (set = 1, binding = 0) uniform uniformBuffer { //binding = 0 because is not dynamic
    Charge charges[2];
} ubo;

vec4 shElectricFieldIntensity(float intensityCoefficient, uint chargeIdx) {
    float r = distance(fragPosition, ubo.charges[chargeIdx].position);
    return vec4(intensityCoefficient * ubo.charges[chargeIdx].color / (r*r));
}

void main(){

    float k = 1.0;
    vec4 fieldIntensity = vec4(0.0);
    for (uint i = 0; i < 2; i++) {
        fieldIntensity += shElectricFieldIntensity(k, i);
    }
    fragColor = fieldIntensity;
}

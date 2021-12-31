#version 460
//INPUTS:
layout (location = 0) flat in mat4 PV;
layout (location = 4) in vec4 fragPosition;

//OUTPUT
layout (location = 0) out vec4 fragColor;

//assign unique color for each body;
//DECLARATIONS/DEFINITIONS:
struct ThermodynamicsBody {
    float mass;
    float density;
    float thermal_expansion_coefficient;
    float specific_heat;
};

layout (set = 0, binding = 0) uniform modelUbo {
    ThermodynamicsBody body[32];
} ubo;

void main(){

    for (uint i = 0; i < 32; i++) {

    }

    fragColor = vec4(1.0f);
}

#version 450
//INPUTS:
layout (location = 1) in vec4 fragPos;

//DECLARATIONS/DEFINITIONS:
struct Charge {
    vec2 position;
    float intensity;
    vec3 colorID;
};

//OUTPUT
layout (location = 0) out vec4 fragColor;

const uint chargeCount = 4;
//RANGE OF VALUES MUST BE BETWEEN 0 AND 1
const Charge charges[chargeCount] = Charge[chargeCount](
    Charge(vec2( 0.0f,-0.4f), 0.6f, vec3(1.0f, 0.0f, 0.0f) ),
    Charge(vec2(-0.3f, 0.0f), 0.8f, vec3(0.0f, 1.0f, 0.0f) ),
    Charge(vec2( 0.7f, 0.0f), 0.1f, vec3(0.0f, 0.0f, 1.0f) ),
    Charge(vec2(-0.5f, 0.5f), 0.3f, vec3(1.0f, 0.0f, 1.0f) )
);

void main(){

    float k = 0.01f; //HIGHER VALUES ARE NOT GOOD, USING 0.01 BECAUSE MAX DISTANCE IS LESS 1.0
    vec3 fieldIntensity = vec3(0.0f);
    for (uint i = 0; i < chargeCount; i++) {
        float r = distance(fragPos, vec4(charges[i].position, 0.0f, 1.0f));
        vec3 intensity = charges[i].colorID*k*charges[i].intensity / (r*r);
        fieldIntensity += intensity;
    }

    fragColor = vec4(fieldIntensity, 1.0f);   
    //fragColor = fragPos;   
}
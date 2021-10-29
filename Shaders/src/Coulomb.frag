#version 460
//INPUTS:
layout (location = 0) flat in mat4 MV;
layout (location = 4) in vec4 fragPosition;

//DECLARATIONS/DEFINITIONS:
struct Charge {
    vec4 position;
    float intensity;
    vec3 colorID;
};

//OUTPUT
layout (location = 0) out vec4 fragColor;

const uint chargeCount = 1;
//RANGE OF VALUES MUST BE BETWEEN 0 AND 1
// The charges are in the scene 3 dimensional space
Charge charges[chargeCount] = Charge[chargeCount](
    Charge(MV * vec4( 0.0f, 0.7f, 0.0f, 1.0f), 10.0f, vec3(1.0f, 0.0f, 0.0f) )
);

void main(){

    float k = 0.01f; //HIGHER VALUES ARE NOT GOOD, USING 0.01 BECAUSE MAX DISTANCE IS LESS 1.0
    vec3 fieldIntensity = vec3(0.0f);
    for (uint i = 0; i < chargeCount; i++) {
        float r = distance(fragPosition, charges[i].position);
        vec3 intensity = charges[i].colorID*k*charges[i].intensity / (r*r);
        fieldIntensity += intensity;
    }

    fragColor = vec4(fieldIntensity, 1.0f);   
    //fragColor = fragPosition;   
}
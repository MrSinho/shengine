#version 460
layout (location = 0) flat in mat4 PV;
layout (location = 4) in vec4 fragPosition;
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
layout (location = 0) out vec4 fragColor;
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
struct ShSphere {
    vec3      position;
    float     radius;
};

uint shSphereSphereIntersection(in ShSphere sphere0, in ShSphere sphere1, out vec3 dstIntersection0, out vec3 dstIntersection1);
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//

struct ShElectricalCharge {
    vec4 position;
    vec4 intensity;
};

layout (set = 1, binding = 0) uniform uniformBuffer {
    ShElectricalCharge charges[32];
} ubo;

vec4 shElectricFieldIntensity(float intensityCoefficient, uint chargeIdx);
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
void main(){

    float k = 0.01;
    vec4 fieldIntensity = vec4(0.0);
    for (uint i = 0; i < 32; i++) {
        fieldIntensity += shElectricFieldIntensity(k, i);
    }
    fragColor = fieldIntensity;
}
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
uint shSphereSphereIntersection(in ShSphere sphere0, in ShSphere sphere1, out vec3 dstIntersection0, out vec3 dstIntersection1) {
    if (sphere0.position == sphere1.position) {
        return 1;
    }
    vec3 dir = sphere1.position - sphere0.position;
    float distance = dir.length();
    if (distance <= (sphere0.radius + sphere1.radius)) {
        vec3 dir0 = normalize(dir) * sphere0.radius;
        vec3 dir1 = normalize(dir) * sphere1.radius;
        dstIntersection0 = vec3(sphere0.position - dir0);
        dstIntersection1 = vec3(sphere1.position + dir1);
        return 1;
    }
    return 0;
}
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
vec4 shElectricFieldIntensity(float intensityCoefficient, uint chargeIdx) {
    float r = distance(fragPosition, PV * vec4(ubo.charges[chargeIdx].position.xyz, 1.0));
    return intensityCoefficient * ubo.charges[chargeIdx].intensity * ubo.charges[chargeIdx].intensity.w / (r*r);
}

//RANGE OF VALUES MUST BE BETWEEN 0 AND 1
// The charges are in the scene 3 dimensional space
/*Charge charges[4] = Charge[4](
    //center
    Charge(PV * vec4( 0.0f, 2.0f, 0.0f, 1.0f), 85.0f, vec3(1.0f, 0.0f, 0.0f) ),
    //left
    Charge(PV * vec4( 1.5f, 0.0f, 0.0f, 1.0f), 85.0f, vec3(0.0f, 1.0f, 0.0f) ),
    //right
    Charge(PV * vec4(-1.5f, 0.0f, 0.0f, 1.0f), 85.0f, vec3(0.0f, 0.0f, 1.0f) ),
    //up
    Charge(PV * vec4( 0.0f,-1.0f, 0.0f, 1.0f), 105.0f, vec3(0.5f, 0.5f, 0.0f) )
);
*/
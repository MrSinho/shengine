#version 460
layout (location = 0) in vec4 fragPosition;
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
layout (location = 0) out vec4 fragColor;
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
/*struct ShSphere {
    vec3      position;
    float     radius;
};

uint shSphereSphereIntersection(in ShSphere sphere0, in ShSphere sphere1, out vec4 dstIntersection0, out vec4 dstIntersection1);
*/
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//

struct ShElectricalCharge {
    vec3 position;
    float intensity;
};

layout (set = 1, binding = 0) uniform uniformBuffer {
    ShElectricalCharge charges[32];
    float dtime;
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
/*uint shSphereSphereIntersection(in ShSphere sphere0, in ShSphere sphere1, out vec4 dstIntersection0, out vec4 dstIntersection1) {
    if (sphere0.position == sphere1.position) {
        return 1;
    }
    vec4 pos0 = vec4(sphere0.position, 1.0);
    vec4 pos1 = vec4(sphere1.position, 1.0);
    vec4 dir =  pos1 - pos0;
    float distance = dir.length();
    if (distance <= (sphere0.radius + sphere1.radius)) {
        vec4 dir0 = normalize(dir) * sphere0.radius;
        vec4 dir1 = normalize(dir) * sphere1.radius;
        dstIntersection0 = vec4(pos0 - dir0);
        dstIntersection1 = vec4(pos1 + dir1);
        return 1;
    }
    return 0;
}*/
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*//
vec4 shElectricFieldIntensity(float intensityCoefficient, uint chargeIdx) {
    float r = distance(fragPosition, vec4(ubo.charges[chargeIdx].position, 1.0));
    return vec4(intensityCoefficient * ubo.charges[chargeIdx].intensity / (r*r));
}
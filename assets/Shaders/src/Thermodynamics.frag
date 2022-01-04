#version 460
//INPUTS:
layout (location = 0) flat in mat4 PV;
layout (location = 4) in vec4 fragPosition;

//OUTPUT
layout (location = 0) out vec4 fragColor;

//COLLISION
struct ShSphere {
    vec3      position;
    float     radius;
};
uint shSphereSphereIntersection(in ShSphere sphere0, in ShSphere sphere1, out vec3 dstIntersection0, out vec3 dstIntersection1);
//HANDLE
struct ThermodynamicsBody {
    float       mass;
    float       density;
    float       thermalExpansionCoefficient;
    float       thermalConductivityCoefficient;
    float       specificHeat;
    float       temperature;
    ShSphere    collisionShape;
};

layout (set = 0, binding = 0) uniform modelUbo {
    ThermodynamicsBody bodies[32];
    uint displayTemperature;
    uint displayHeat;
} ubo;

void main(){
    //do for each fragment the distance from the bodies
    for (uint i = 0; i < 32; i++) {
        for (uint j = 0; j < 32; j++) { 
            if (i != j) {
                float Q0 = bodies[i].mass * bodies[i].specificHeat * bodies[i].temperature;
                float Q1 = bodies[j].mass * bodies[j].specificHeat * bodies[j].temperature;
                if (shSphereSphereIntersection(bodies[i].collisionShape, bodies[j].collisionShape, vec3(0.0), vec3(0.0))) {
                    //fourier's law
                }
            }
        }
    }

    fragColor = vec4(1.0f);
}

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
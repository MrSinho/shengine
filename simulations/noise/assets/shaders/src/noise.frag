#version 450

layout (location = 0) in vec4 frag_position;
layout (location = 0) out vec4 fragColor;

layout (std140, set = 0, binding = 0) uniform uFractalProperties {
    float zoom;
    vec2 rotation;
} ufrac;

void main() {

    if (ufrac.rotation.x == 0.0 || ufrac.rotation.y == 0.0 || frag_position.y == 0.0) {
        fragColor = vec4(1.0);
        return;
    }

    fragColor = vec4(
        normalize(
            vec3(
                sin(3.14 * abs(frag_position.x) * ufrac.zoom       / ufrac.rotation.x),
                sin(3.14 * abs(frag_position.y) * ufrac.zoom       / ufrac.rotation.y), 
                cos(3.14 * abs(frag_position.x  / frag_position.y) * ufrac.zoom)
            )
        ),
        1.0f
    );   
}
#version 450

layout (location = 0) in vec4 frag_position;
layout (location = 0) out vec4 fragColor;


void main() {
    fragColor = vec4(
        normalize(
            vec3(
                sin(3.14       * abs(frag_position.x)),
                sin(3.14 / 2   * abs(frag_position.y)), 
                cos(3.14 * 2/3 * abs(frag_position.x) / frag_position.y)
            )
        ),
        1.0f
    );   
}
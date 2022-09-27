#version 450

layout (location = 0) in vec4 frag_position;
layout (location = 0) out vec4 fragColor;


void main() {
    fragColor = vec4(
        normalize(
            vec3(
                0.8 + 0.6 * sin(frag_position.x * 2.0f),
                0.6 + 0.4 * sin(frag_position.y), 
                0.9 + 0.6 * sin(frag_position.y)
            )
        ),
        1.0f
    );   
}
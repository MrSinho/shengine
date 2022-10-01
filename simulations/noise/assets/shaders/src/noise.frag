#version 450

layout (location = 0) in vec4 frag_position;
layout (location = 0) out vec4 fragColor;

layout (std140, set = 0, binding = 0) uniform uFractalProperties {
    float s;
    float a;
    float b;
} ufrac;

#define pi 3.1415926535

void main() {

    if (ufrac.a == 0.0 || ufrac.b == 0.0 || frag_position.y == 0.0) {
        fragColor = vec4(1.0);
        return;
    }

    fragColor = vec4(
        normalize(
            vec3(
                sin(pi * abs(frag_position.x) * ufrac.s / ufrac.a),
                sin(pi * abs(frag_position.y) * ufrac.s / ufrac.b),
                cos(pi * frag_position.x / frag_position.y * ufrac.s)    
            ) + 
            vec3(
                cos(pi * frag_position.x * frag_position.y * ufrac.s)
            )
        ),
        1.0f
    );
}

/*
RGB = \begin{bmatrix}

\sin(\pi |x| \cfrac{s}{a})\\
\sin(\pi |y| \cfrac{s}{b})\\
\cos(\pi \cfrac{x}{y}s)\\

\end{bmatrix}

+

\cos(\pi xys)
*/

/*

*/
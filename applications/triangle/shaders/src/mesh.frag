#version 460

layout (location = 0) in vec4 frag_position;
layout (location = 0) out vec4 frag_color;



void main(){
    frag_color = vec4(
        frag_position.xy, 
        0.5, 
        1.0
    );
}
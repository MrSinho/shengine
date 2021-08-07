#version 450

layout (location = 0) in vec4 fragPosition;
//layout (location = 1) in vec3 fragNormal;
//layout (location = 2) in vec2 fragUV;

layout (location = 0) out vec4 fragColor;


void main(){
    fragColor = fragPosition;   
}
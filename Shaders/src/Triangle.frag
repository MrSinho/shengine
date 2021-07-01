#version 450

layout (location = 1) in vec4 color;

layout (location = 0) out vec4 fragColor;

void main(){
    fragColor = color;   
}
#version 450

layout (location = 0) in vec4 color;

layout (location = 0) out vec4 fragPosition;


void main(){
    fragPosition = color;   
}
#version 450

vec2 positions[3] = vec2[] (
	vec2(0, -1),	//up
	vec2(-1, 1),	//left
	vec2(1, -1)		//right		
);

layout (location = 0) out vec4 fragmentPosition;

void main() {
	fragmentPosition = vec4(positions[gl_VertexIndex], 0.0, 1.0);;
	gl_Position = fragmentPosition;
}
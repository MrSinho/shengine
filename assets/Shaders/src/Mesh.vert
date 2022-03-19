#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 uvs;

layout (location = 0) out vec4 fragPosition;
//layout (location = 2) out vec3 fragNormal;
//layout (location = 1) out vec2 fragUV;

layout (push_constant) uniform constants {
	mat4 projection;
	mat4 view;
} pconst;

layout (set = 0, binding = 1) uniform uniformBuffer { //binding = 1 because is dynamic
    mat4 model;
} ubo;


void main() {
  gl_Position = pconst.projection * pconst.view * ubo.model * vec4(position, 1.0);
}
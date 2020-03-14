#version 410 core

layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texCoords;

out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;

// again, not exactly efficient. UBOs are probably the way to go
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    texCoords = i_texCoords;
    normal = (model * vec4(i_normal, 1.f)).xyz;
    fragPos = (model * vec4(i_position, 1.f)).xyz;
    gl_Position = proj * view * model * vec4(i_position, 1.f);
}

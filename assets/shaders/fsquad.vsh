#version 410 core

layout (location = 0) in vec2 i_position;
layout (location = 1) in vec2 i_texCoord;

out vec2 texCoord;

void main() {
    texCoord = i_texCoord;
    gl_Position = vec4(i_position, 0, 1);
}

#version 410 core
in vec2 texCoord;
out vec4 fragColor;

struct Light {
    vec3 position;
    vec3 color;
};

struct Camera {
    vec3 position;
    vec3 target;
};

uniform sampler2D color;
uniform sampler2D depth;
uniform Camera camera;
uniform Light light;
uniform vec2 resolution;

void main() {
    fragColor = texture(color, texCoord);
}

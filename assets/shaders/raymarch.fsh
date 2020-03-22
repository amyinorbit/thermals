#version 410 core
out vec4 fragColor;

struct Light {
    vec3 position;
    vec3 color;
};

struct Camera {
    vec3 position;
    vec3 target;
};

uniform sampler2D tex;
uniform Camera camera;
uniform Light light;
uniform vec2 resolution;

void main() {
    fragColor = vec4(gl_FragCoord.xy / resolution, 0.f, 1.f);
}

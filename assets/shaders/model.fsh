#version 410 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

struct Light {
    vec3 direction;
    vec3 color;
};

struct Camera {
    vec3 position;
    vec3 target;
};

// This should really be moved to a Uniform Buffer Object
layout (location = 0) uniform Camera camera;
layout (location = 2) uniform Light light;

uniform sampler2D tex;

void main() {
    vec3 norm = normalize(normal);

    vec3 diffuse = max(dot(light.direction, norm), 0.f) * light.color;
    vec3 ambiant = 0.1f * light.color;
    fragColor = (ambiant + diffuse) * texture(tex, texCoord);
}

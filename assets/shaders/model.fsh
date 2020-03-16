#version 410 core

in vec2 texCoord;
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
uniform Camera camera;
uniform Light light;

uniform sampler2D tex;

void main() {

    vec3 norm = normalize(normal);

    vec3 diffuse = max(dot(light.direction, norm), 0.f) * light.color;
    vec3 ambiant = 0.1f * light.color;

    vec4 solidColor = vec4(0.5f, 0.7f, 0.8f, 1.f);

    fragColor = vec4(ambiant + diffuse, 1.f) * solidColor;//texture(tex, texCoord);
}

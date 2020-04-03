#version 410 core

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

struct Light {
    vec3 position;
    vec3 color;
};

struct Camera {
    vec3 position;
    vec3 target;
};

// This should really be moved to a Uniform Buffer Object
uniform sampler2D tex;
uniform Camera camera;
uniform Light light;
uniform float blend;

void main() {

    vec3 norm = normalize(normal);

    vec3 lightDir = normalize(light.position - fragPos);

    float illum = dot(lightDir, norm);
    if(illum < 0.f) illum *= -0.6; // this allows a cheap "subsurface scattering-ish" effect

    vec3 diffuse = illum * light.color;
    vec3 ambiant = 0.3f * light.color;

    vec4 solidColor = vec4(1);
    vec4 color = mix(solidColor, texture(tex, texCoord), blend);
    fragColor = color * vec4(ambiant + diffuse, 1);
}

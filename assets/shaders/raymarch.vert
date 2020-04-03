#version 410 core

struct Camera {
    vec3 position;
    vec3 target;
    float fov;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

layout (location = 0) in vec2 i_position;
out Ray ray;

uniform Camera camera;
uniform vec2 resolution;

Ray castRay(vec2 vert) {
    float aspect = resolution.x / resolution.y;
    float tanHalfFov = tan(radians(camera.fov)/2);
    vec3 forward = normalize(camera.target - camera.position);
    vec3 up = vec3(0, 1, 0);
    vec3 right = normalize(cross(forward, up));
    up = cross(right, forward);

    return Ray(
        camera.position,
        forward + vert.x * aspect * right * tanHalfFov + vert.y * up * tanHalfFov
    );
}

void main() {
    gl_Position = vec4(i_position, 0, 1);
    ray = castRay(i_position);
}

#version 410 core

struct Light {
    vec3 position;
    vec3 color;
};

struct Camera {
    vec3 position;
    vec3 target;
    float fov;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

in Ray ray;
out vec4 fragColor;

uniform Camera camera;
uniform Light light;
uniform vec2 resolution;
uniform mat4 projection;
uniform mat4 view;
uniform float time;

float sdf(vec3 p) {
    vec3 center = vec3(0, 1, 0);
    return distance(p, center) - 1.2;
}

#define STEPS 64
#define MIN_DISTANCE 0.0001
vec4 raymarch(vec3 origin, vec3 direction) {
    vec3 pos = origin;
    mat4 pv = projection * view;
    for(int i = 0; i < STEPS; ++i) {
        float dist = sdf(pos);
        if(dist < MIN_DISTANCE) {
            vec4 dv = pv * vec4(pos, 1);
            float depth = 0.5 * ((dv.z/dv.w) + 1.0);
            gl_FragDepth = depth;
            return vec4(1.f/depth, 1, 1, 1);
        }
        pos += dist * direction;
    }
    discard;
}

void main() {
    // fragColor = vec4(gl_FragDepth);
    fragColor = raymarch(ray.origin, ray.direction);
    // fragColor = vec4(ray.direction, .5f);
}

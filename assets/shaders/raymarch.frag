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


float sdBox( vec3 p, vec3 b ) {
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdSphere(vec3 p, vec3 center, float radius) {
    return distance(p, center) - radius;
}

float scene(vec3 p) {
    float sun = sdSphere(p, light.position, 10);
    return sun;
}

#define STEPS 64
#define MIN_DISTANCE 0.01
vec4 raymarch(vec3 origin, vec3 direction) {
    vec3 pos = origin;
    mat4 pv = projection * view;
    for(int i = 0; i < STEPS; ++i) {
        float dist = scene(pos);
        if(dist < MIN_DISTANCE) {
            vec4 dv = pv * vec4(pos, 1);
            float depth = 0.5 * ((dv.z/dv.w) + 1.0);
            gl_FragDepth = depth;
            return vec4(1.f/depth, 1, 1, 1);
        }
        pos += dist * direction;
    }
    return vec4(0);
}

void main() {
    fragColor = raymarch(ray.origin, ray.direction);
}

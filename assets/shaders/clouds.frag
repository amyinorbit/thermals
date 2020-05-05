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

in vec2 texCoord;
in Ray ray;
out vec4 fragColor;

uniform sampler2D color;
uniform sampler2D depth;
uniform sampler3D noise;
uniform sampler2D clouds;

uniform Camera camera;
uniform Light light;
uniform vec2 resolution;
uniform mat4 projection;
uniform mat4 view;
uniform float time;
uniform float wind_speed;

const vec3 wind_dir = vec3(0.01f, 0.f, 0.f);

float remap(float x, float i_min, float i_max, float o_min, float o_max) {
    return o_min + (x - i_min)*(o_max-o_min)/(i_max-i_min);
}

float height(float y, float alt, float dev) {
    return exp(- pow(y-alt, 2.f) / (2.f * pow(dev, 2.f)));
}

// "basic" density function. We use the coverage map, along with a height barrier,
// and the "carve out" with the 3d noise texture (see Nubis papers)
float density(vec3 p) {
    vec3 texcoord = (p / 20.f) + vec3(0.5);

    float h = height(p.y, 8, 1.5);
    float noiseValue = texture(noise, texcoord.xzy + time * wind_speed * wind_dir).r;
    float coverageValue = remap(texture(clouds, texcoord.xz).r, 0.f, 1.f, 0.f, 1.f);

    return remap(noiseValue, coverageValue, 1.f, 0.f, 1.f) * h;
}

#define MAX_STEPS 200
#define EPSILON 1e-3
#define INV_EPSILON (1 - EPSILON)
#define TAU 5.f

#define STEP_SIZE 0.2f
// #define STEP_FINE 0.1f

float beerLambert(float density, float distance) {
    return exp(-TAU * density * distance);
}

float cloudOpacity(vec3 origin, vec3 direction, float sceneDepth) {

    vec3 pos = origin + EPSILON * direction;
    float trans = 1.f;
    float stepSize = STEP_SIZE;
    mat4 pv = projection * view;

    bool hasHit = false;

    for(int i = 0; i < MAX_STEPS; ++i) {
        float dt = beerLambert(density(pos), stepSize);

        trans *= dt;
        if(trans < EPSILON) break;
        vec4 dv = pv * vec4(pos, 1);
        float curDepth = 0.5 * ((dv.z/dv.w) + 1.0);
        if(curDepth > sceneDepth) break;
        pos += stepSize * direction;
    }
    return clamp(1-trans, 0, 1);
}

void main() {
    vec3 sceneColor = texture(color, texCoord).rgb;
    float opacity = cloudOpacity(ray.origin, ray.direction, texture(depth, texCoord).r);
    vec3 total = mix(sceneColor, vec3(1), opacity);
    fragColor = vec4(total, 1);
}

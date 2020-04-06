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

uniform sampler3D noise;
uniform sampler2D clouds;

uniform Camera camera;
uniform Light light;
uniform vec2 resolution;
uniform mat4 projection;
uniform mat4 view;
uniform float time;



float remap(float x, float i_min, float i_max, float o_min, float o_max) {
    return o_min + (x - i_min)*(o_max-o_min)/(i_max-i_min);
}

float height(float y, float alt, float dev) {
    return exp(- pow(y-alt, 2.f) / (2.f * pow(dev, 2.f)));
}

float bubble(vec2 pos) {
    float d = distance(pos, vec2(5, -10));
    return exp(- pow(d, 2.f) / (2.f * pow(2.f, 2.f)));
}

// "basic" density function. We use the coverage map, along with a height barrier,
// and the "carve out" with the 3d noise texture (see Nubis papers)
float density(vec3 p) {
    vec3 texcoord = (p / 200.f) + vec3(1.5);
    // For debug, let's just make one big ol' circular cloud
    float h = height(p.y, 25, 3);
    float noiseValue = texture(noise, texcoord.xzy * 3).r;
    float coverageValue = remap(texture(clouds, texcoord.xz).r, 0.3f, 1.f, 0.f, 1.f);
    return coverageValue * noiseValue * h;
}

#define MAX_STEPS 64
#define EPSILON 1e-3
#define TAU 10.f

// We use these to have a ramp, the closer to the camera we are, the more precise we
// want to be with our sampling. At least I hope.
#define START_STEP 0.01f
#define END_STEP 5.f

float beerLambert(float density, float distance) {
    return exp(-TAU * density * distance);
}

vec4 raymarch(vec3 origin, vec3 direction) {

    vec3 pos = origin + EPSILON * direction;
    float trans = 1.f;
    bool hit = false;
    float stepSize = START_STEP;

    for(int i = 0; i < MAX_STEPS; ++i) {
        float dt = beerLambert(density(pos), stepSize);
        stepSize = mix(START_STEP, END_STEP, float(i)/float(MAX_STEPS));

        if(!hit && (1-dt) > EPSILON) {
            hit = true;
            mat4 pv = projection * view;
            vec4 dv = pv * vec4(pos, 1);
            gl_FragDepth = 0.5 * ((dv.z/dv.w) + 1.0);
        }

        trans *= dt;
        if(trans < EPSILON) break;
        pos += stepSize * direction;
    }
    return vec4(1, 1, 1, 1-trans);
}

void main() {
    fragColor = raymarch(ray.origin, ray.direction);
}

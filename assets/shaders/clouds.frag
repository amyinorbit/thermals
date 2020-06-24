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
    return clamp(o_min + (x - i_min)*(o_max-o_min)/(i_max-i_min), o_min, o_max);
}

float height(float y, float alt, float dev) {
    return exp(- pow(y-alt, 2.f) / (2.f * pow(dev, 2.f)));
}

// "basic" density function. We use the coverage map, along with a height barrier,
// and the "carve out" with the 3d noise texture (see Nubis papers)
float density(vec3 p) {
    vec3 texcoord = (p / 20.f) + vec3(0.5);
    float h = height(p.y, 8, 1.5);
    float noiseValue = texture(noise, (texcoord.xzy) + time * wind_speed * wind_dir).r;
    float coverageValue = texture(clouds, texcoord.xz).r;
    return remap(noiseValue, coverageValue, 1.f, 0.f, 1.f) * h;
}

#define PI 3.14

// This is the "intro" to light scattering
//
float HGscattering(float cosAngle, float eccentricity) {
    return ((1.0 - eccentricity * eccentricity) / pow((1.0 + eccentricity * eccentricity - 2.0 * eccentricity * cosAngle), 3.0 / 2.0)) / 4.0 * PI;
}

float scattering(vec3 dir, vec3 p) {
    vec3 lightDir = p - light.position;
    float cosAngle = dot(normalize(lightDir), normalize(dir));


    return HGscattering(cosAngle, 0.9);
}

#define MAX_STEPS 400
#define EPSILON 1e-3
#define INV_EPSILON (1 - EPSILON)
#define TAU 10.f

#define STEP_SIZE 0.2f
// #define STEP_FINE 0.1f

float beerLambert(float density, float distance) {
    return exp(-TAU * density * distance);
}

vec4 cloudOpacity(vec3 origin, vec3 direction, float sceneDepth) {

    vec3 pos = origin + EPSILON * direction;
    vec3 cloud_color = vec3(0.0f);
    float trans = 1.f;
    float stepSize = STEP_SIZE;
    mat4 pv = projection * view;

    bool hasHit = false;

    for(int i = 0; i < MAX_STEPS; ++i) {
        float dt = beerLambert(density(pos), stepSize);

        trans *= dt;
        cloud_color += 0.1 * scattering(direction, origin) * light.color;
        if(trans < EPSILON) break;
        vec4 dv = pv * vec4(pos, 1);
        float curDepth = 0.5 * ((dv.z/dv.w) + 1.0);
        if(curDepth > sceneDepth) break;
        pos += stepSize * direction;
    }
    return vec4(cloud_color, clamp(1-trans, 0, 1));
}

void main() {
    vec3 sceneColor = texture(color, texCoord).rgb;
    vec4 cloud = cloudOpacity(ray.origin, ray.direction, texture(depth, texCoord).r);
    vec3 total = mix(sceneColor, cloud.rgb, cloud.a);
    fragColor = vec4(total, 1);
}

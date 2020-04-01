#version 410 core
in vec2 texCoord;
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


    const float offset = 1.0 / 300.0;
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(tex, texCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    fragColor = texture(tex, texCoord);
    // fragColor = mix(texture(tex, texCoord), vec4(0, 0, 0, 1), length(col.rgb));//texture(tex, texCoord);
}

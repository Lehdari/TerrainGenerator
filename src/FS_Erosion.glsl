/**
    FS_Erosion.glsl

    @version    0.1
    @author     Miika 'LehdaRi' Lehtimäki
    @date       2015-06-18
**/


#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D tex;

const float a = 1.0/256;

float calculateWaterLevel(vec2 UV) {
    vec2 l[5];
    float w = 0.0;
    float d = 1.0;

    l[0] = texture(tex, vec2(UV.x, UV.y)).rb;
    w += l[0].x + l[0].y;

    if (UV.x-a >= 0.0) {
        l[1] = texture(tex, vec2(UV.x-a, UV.y)).rb;
        w += l[1].x + l[1].y;
        d += 1.0;
    }
    if (UV.x+a >= 0.0) {
        l[2] = texture(tex, vec2(UV.x+a, UV.y)).rb;
        w += l[2].x + l[2].y;
        d += 1.0;
    }
    if (UV.y-a >= 0.0) {
        l[3] = texture(tex, vec2(UV.x, UV.y-a)).rb;
        w += l[3].x + l[3].y;
        d += 1.0;
    }
    if (UV.y+a >= 0.0) {
        l[4] = texture(tex, vec2(UV.x, UV.y+a)).rb;
        w += l[4].x + l[4].y;
        d += 1.0;
    }

    w /= d;
    w -= l[0].x;
    if (w < 0.0)
        w = 0.0;

    return w;
}

void main() {
    vec2 p = UV * 256;

    color = texture(tex, UV);

    color.b = calculateWaterLevel(UV);

    if (int(p.x) == 128 && int(p.y) == 90)
        color.b += 0.01;
}


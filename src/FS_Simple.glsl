/**
    FS_Simple.glsl

    @version    0.1
    @author     Miika 'LehdaRi' Lehtimäki
    @date       2015-04-18
**/


#version 330 core

varying vec2 UV;

out vec4 color;

uniform sampler2D tex;

const float a = 1.0/512;

int checkType(vec3 col) {
    if (col == vec3(1.0, 1.0, 1.0)) return 1;
    if (col == vec3(0.0, 0.0, 1.0)) return 2;
    if (col == vec3(0.0, 1.0, 1.0)) return 3;
    if (col == vec3(1.0, 1.0, 0.0)) return 4;
    if (col == vec3(0.0, 1.0, 0.0)) return 5;
    return 0;
}

void main() {
    color = texture(tex, UV);

    vec3 left = vec3(0.0, 0.0, 0.0);
    if (UV.x-a >= 0.0)
        left = texture(tex, vec2(UV.x-a, UV.y)).rgb;

    vec3 right = vec3(0.0, 0.0, 0.0);
    if (UV.x+a >= 0.0)
        right = texture(tex, vec2(UV.x+a, UV.y)).rgb;

    vec3 above = vec3(0.0, 0.0, 0.0);
    if (UV.y-a >= 0.0)
        above = texture(tex, vec2(UV.x, UV.y-a)).rgb;

    vec3 below = vec3(0.0, 0.0, 0.0);
    if (UV.y+a >= 0.0)
        below = texture(tex, vec2(UV.x, UV.y+a)).rgb;

    int leftType = checkType(left);
    int rightType = checkType(right);
    int aboveType = checkType(above);
    int belowType = checkType(below);

    /*if (color == vec4(1.0, 1.0, 1.0, 1.0) || color == vec4(0.0, 0.0, 1.0, 1.0)) {
        unsigned int n = 0;
        if (leftType == 1 || leftType == 2 || leftType == 5) ++n;
        if (rightType == 1 || rightType == 2 || leftType == 5) ++n;
        if (aboveType == 1 || aboveType == 2 || leftType == 5) ++n;
        if (belowType == 1 || belowType == 2 || leftType == 5) ++n;

        if (n <= 1) {
            color = vec4(0.5, 0.5, 0.5, 1.0);
            return;
        }
    }*/

    if (color == vec4(1.0, 1.0, 1.0, 1.0)) {
        if (leftType == 4 || leftType == 3 ||
            rightType == 4 || rightType == 3 ||
            aboveType == 4 || aboveType == 3 ||
            belowType == 4 || belowType == 3) {
            color = vec4(0.0, 1.0, 1.0, 1.0);
            return;
        }
    }

    if (color == vec4(0.0, 1.0, 1.0, 1.0)) {
        if (leftType == 4 || leftType == 5 ||
            rightType == 4 || rightType == 5 ||
            aboveType == 4 || aboveType == 5 ||
            belowType == 4 || belowType == 5) {
            color = vec4(0.0, 0.0, 1.0, 1.0);
            return;
        }
        else {
            unsigned int n = 0;
            if (leftType == 2) ++n;
            if (rightType == 2) ++n;
            if (aboveType == 2) ++n;
            if (belowType == 2) ++n;

            if (n == 1) {
                color = vec4(0.0, 0.0, 1.0, 1.0);
                return;
            }

            color = vec4(0.5, 0.5, 0.5, 1.0);
            return;
        }
    }

    if (color == vec4(0.0, 0.0, 1.0, 1.0)) {
        if (leftType == 5 ||
            rightType == 5 ||
            aboveType == 5 ||
            belowType == 5) {
            color = vec4(0.0, 1.0, 0.0, 1.0);
            return;
        }

        unsigned int n = 0;
        if (leftType == 1 || leftType == 2 || leftType == 3 || leftType == 4) ++n;
        if (rightType == 1 || rightType == 2 || rightType == 3 || rightType == 4) ++n;
        if (aboveType == 1 || aboveType == 2 || aboveType == 3 || aboveType == 4) ++n;
        if (belowType == 1 || belowType == 2 || belowType == 3 || belowType == 4) ++n;

        if (n <= 1) {
            color = vec4(0.5, 0.5, 0.5, 1.0);
            return;
        }
    }
}

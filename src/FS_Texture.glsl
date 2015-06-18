/**
    FS_Texture.glsl

    @version    0.1
    @author     Miika 'LehdaRi' Lehtimäki
    @date       2015-06-18
**/


#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D tex;

void main() {
    color = texture(tex, UV);
    //color = vec4(UV, 0.0, 1.0);
}

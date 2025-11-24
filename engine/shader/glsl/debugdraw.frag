#version 450

#extension GL_GOOGLE_include_directive :enable
layout(location = 0) in vec4 fragColor;

layout(location = 0) out vec4 outColor;

void main(){
    outColor = fragColor;
}
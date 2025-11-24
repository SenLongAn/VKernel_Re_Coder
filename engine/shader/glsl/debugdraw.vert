#version 450

#extension GL_GOOGLE_include_directive :enable // Enable the GL_GOOGLE_include_directive extension to allow the use of the #include directive in GLSL shaders.
#include "constants.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = vec4(inPosition,1.0);

    fragColor = inColor;
}
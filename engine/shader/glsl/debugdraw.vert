#version 450

#extension GL_GOOGLE_include_directive :enable // Enable the GL_GOOGLE_include_directive extension to allow the use of the #include directive in GLSL shaders.
#include "constants.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model [2];
} ubo;

layout(location = 0) out vec4 fragColor;

void main() {
    int triangleIndex = gl_VertexIndex / 3;
    gl_Position = ubo.model[triangleIndex] * vec4(inPosition,1.0);

    fragColor = inColor;
}
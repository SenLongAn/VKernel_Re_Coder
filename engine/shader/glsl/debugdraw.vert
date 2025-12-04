#version 450

#extension GL_GOOGLE_include_directive :enable // Enable the GL_GOOGLE_include_directive extension to allow the use of the #include directive in GLSL shaders.
#include "constants.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 proj_view_matrix;
} ubo;

layout(set = 0, binding = 1) uniform UniformDynamicBufferObject {
    mat4 model;
} dynamic_ubo;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = ubo.proj_view_matrix * dynamic_ubo.model * vec4(inPosition,1.0);

    fragColor = inColor;
}
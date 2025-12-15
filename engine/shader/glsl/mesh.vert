#version 310 es

#extension GL_GOOGLE_include_directive : enable // Enable extension

#include "constants.h"

layout(set = 0, binding = 0) readonly buffer _unused_name_perframe { mat4 proj_view_matrix; };

layout(set = 0, binding = 1) readonly buffer _unused_name_per_drawcall { mat4 model_matrix; };

layout(location = 0) in vec3 in_position; // for some types as dvec3 takes 2 locations

void main() { gl_Position = proj_view_matrix * model_matrix * vec4(in_position, 1.0); }

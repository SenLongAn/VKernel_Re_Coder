#version 450 core
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_geometry_shader : enable
#include "constants.h"

layout(set = 0, binding = 0) readonly buffer _unused_name_global_set_per_frame_binding_buffer
{
    uint       point_light_count;
    uint       _padding_point_light_count_0;
    uint       _padding_point_light_count_1;
    uint       _padding_point_light_count_2;
    highp vec4 point_lights_position_and_radius[s_max_point_light_count];
    highp mat4 point_light_matrices[s_max_point_light_count * 6];
};

// in
layout(location = 0) in highp vec3 FragPos;

// out
layout(location = 0) out highp float out_depth;

void main()
{
    // calculation depth
    out_depth = length(point_lights_position_and_radius[gl_Layer / 6].xyz - FragPos);
}
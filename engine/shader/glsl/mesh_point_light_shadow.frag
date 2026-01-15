#version 310 es
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_geometry_shader : enable
#include "constants.h"

layout(set = 0, binding = 0) readonly buffer _unused_name_global_set_per_frame_binding_buffer
{
    uint       point_light_count;
    uint       _padding_point_light_count_0;
    uint       _padding_point_light_count_1;
    uint       _padding_point_light_count_2;
    highp vec4 point_lights_position_and_radius[m_max_point_light_count];
};

// in
layout(location = 0) in highp float in_inv_length;
layout(location = 1) in highp vec3 in_inv_length_position_view_space;

// out
layout(location = 0) out highp float out_depth;

void main()
{
    // calculation depth
    highp vec3 position_view_space = in_inv_length_position_view_space / in_inv_length;

    highp int   point_light_index  = gl_Layer / 2;
    highp float point_light_radius = point_lights_position_and_radius[point_light_index].w;

    highp float distance_from_light = length(position_view_space);
    highp float depth_value         = distance_from_light / point_light_radius;

    if (depth_value > 1.0)
    {
        discard;
    }

    gl_FragDepth = depth_value;
    out_depth    = depth_value;
}
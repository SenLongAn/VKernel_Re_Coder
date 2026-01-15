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
    highp vec4 point_lights_position_and_radius[s_max_point_light_count];
    highp mat4 point_light_matrices[s_max_point_light_count * 6];
};

// in: triangle
layout(triangles) in;
layout(triangle_strip, max_vertices = m_max_point_light_geom_vertices) out;

// in: Vertex world position
layout(location = 0) in highp vec3 in_positions_world_space[];

layout(location = 0) out highp vec3 FragPos;

void main()
{
    for (highp int point_light_index = 0;
         point_light_index < int(point_light_count) && point_light_index < s_max_point_light_count;
         ++point_light_index)
    {
        for (int face = 0; face < 6; ++face)
        {
            gl_Layer = point_light_index * 6 + face;
            for (int i = 0; i < 3; ++i)
            {
                highp vec4 world_pos = vec4(in_positions_world_space[i], 1.0);
                FragPos              = in_positions_world_space[i];
                gl_Position          = point_light_matrices[point_light_index * 6 + face] * world_pos;
                EmitVertex();
            }
            EndPrimitive();
        }
    }
}
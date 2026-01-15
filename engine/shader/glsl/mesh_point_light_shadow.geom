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

// in: triangle
layout(triangles) in;
layout(triangle_strip, max_vertices = m_max_point_light_geom_vertices) out;

// in: Vertex world position
layout(location = 0) in highp vec3 in_positions_world_space[];

// out
layout(location = 0) out highp float out_inv_length;
layout(location = 1) out highp vec3 out_inv_length_position_view_space;

void main()
{
    // Traverse each light source
    for (highp int point_light_index = 0;
         point_light_index < int(point_light_count) && point_light_index < m_max_point_light_count;
         ++point_light_index)
    {
        // current light position and radius
        vec3  point_light_position = point_lights_position_and_radius[point_light_index].xyz;
        float point_light_radius   = point_lights_position_and_radius[point_light_index].w;

        // Calculation layer
        vec3 triangle_center =
            (in_positions_world_space[0] + in_positions_world_space[1] + in_positions_world_space[2]) / 3.0;
        vec3 light_to_center = triangle_center - point_light_position;

        int target_layer = (light_to_center.z >= 0.0) ? 1 : 0;

        // Traverse each vertex
        for (highp int vertex_index = 0; vertex_index < 3; ++vertex_index)
        {
            // calculation vertex position
            highp vec3 position_world_space = in_positions_world_space[vertex_index];
            highp vec3 position_view_space  = position_world_space - point_light_position;
            highp vec3 position_spherical   = normalize(position_view_space);

            highp vec4 position_clip;

            if (target_layer == 1)
            {
                highp float w    = position_spherical.z + 1.0;
                position_clip.xy = position_spherical.xy / w;
                position_clip.w  = w;
            }
            else
            {
                highp float w    = -position_spherical.z + 1.0;
                position_clip.xy = position_spherical.xy / w;
                position_clip.w  = w;
            }

            position_clip.z = 0.5 * position_clip.w;
            gl_Position     = position_clip;

            // out
            out_inv_length                     = 1.0f / length(position_view_space);
            out_inv_length_position_view_space = out_inv_length * position_view_space;

            // layer
            gl_Layer = target_layer + 2 * point_light_index;

            EmitVertex();
        }
        EndPrimitive();
    }
}
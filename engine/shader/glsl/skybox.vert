#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"

struct DirectionalLight
{
    highp vec3 direction;
    lowp float _padding_direction;
    highp vec3 color;
    lowp float _padding_color;
};

struct PointLight
{
    vec3  position;
    float radius;
    vec3  intensity;
    float _padding_intensity;
};

layout(set = 0, binding = 0) readonly buffer _skybox_per_frame
{
    mat4             proj_view_matrix;
    vec3             camera_position;
    float            _padding_camera_position;
    vec3             ambient_light;
    float            _padding_ambient_light;
    uint             point_light_num;
    uint             _padding_point_light_num_1;
    uint             _padding_point_light_num_2;
    uint             _padding_point_light_num_3;
    PointLight       scene_point_lights[s_max_point_light_count];
    DirectionalLight scene_directional_light;
    highp mat4       directional_light_proj_view;
};

// out
layout(location = 0) out vec3 out_UVW;

void main()
{
    // vertex data
    const vec3 cube_corner_vertex_offsets[8] = vec3[8](vec3(1.0, 1.0, 1.0),
                                                       vec3(1.0, 1.0, -1.0),
                                                       vec3(1.0, -1.0, -1.0),
                                                       vec3(1.0, -1.0, 1.0),
                                                       vec3(-1.0, 1.0, 1.0),
                                                       vec3(-1.0, 1.0, -1.0),
                                                       vec3(-1.0, -1.0, -1.0),
                                                       vec3(-1.0, -1.0, 1.0));

    // indice data
    const int cube_triangle_index[36] = int[36](
        0, 1, 2, 2, 3, 0, 4, 5, 1, 1, 0, 4, 7, 6, 5, 5, 4, 7, 3, 2, 6, 6, 7, 3, 4, 0, 3, 3, 7, 4, 1, 5, 6, 6, 2, 1);

    // current vertex data
    vec3 world_position = camera_position + cube_corner_vertex_offsets[cube_triangle_index[gl_VertexIndex]];

    // world to NDC
    vec4 clip_position = proj_view_matrix * vec4(world_position, 1.0);

    // z = w
    clip_position.z = clip_position.w * 0.99999;
    gl_Position     = clip_position;

    // out : Direction vector
    out_UVW = normalize(world_position - camera_position);
}
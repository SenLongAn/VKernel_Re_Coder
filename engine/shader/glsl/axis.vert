#version 460 core

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"

struct PointLight
{
    vec3  position;
    float radius;
    vec3  intensity;
    float _padding_intensity;
};

struct DirectionalLight
{
    vec3  direction;
    float _padding_direction;
    vec3  color;
    float _padding_color;
};

layout(set = 0, binding = 0) readonly buffer _unused_name_perframe
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
    PointLight       scene_point_lights[m_max_point_light_count];
    DirectionalLight scene_directional_light;
    highp mat4       directional_light_proj_view;
};

layout(set = 0, binding = 1) readonly buffer _unused_name_axis
{
    mat4 model_matrix;
    uint selected_axis; ///< Currently selected axis index
};

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_texcoord;

layout(location = 0) out vec3 out_color;

void main()
{
    // MVP
    vec3 world_position = (model_matrix * vec4(in_position, 1.0)).xyz;
    vec4 clip_position  = proj_view_matrix * vec4(world_position, 1.0f);

    // depth == 0, Bring the gizmo to the front
    clip_position.z = clip_position.z * 0.0001;
    gl_Position     = clip_position;

    // Set the axis color according to the axis's texture coordinates
    if (in_texcoord.x < 0.01f)
    {
        if (selected_axis == 0) ///< Show yellow when the mouse hovers
        {
            out_color = vec3(1.0, 1.0, 0.0);
        }
        else
        {
            out_color = vec3(1.0, 0.0, 0.0); ///< red
        }
    }
    else if (in_texcoord.x < 1.01f)
    {
        if (selected_axis == 1)
        {
            out_color = vec3(1.0, 1.0, 0.0);
        }
        else
        {
            out_color = vec3(0.0, 1.0, 0.0); ///< green
        }
    }
    else if (in_texcoord.x < 2.01f)
    {
        if (selected_axis == 2)
        {
            out_color = vec3(1.0, 1.0, 0.0);
        }
        else
        {
            out_color = vec3(0.0, 0.0, 1.0); ///< blue
        }
    }
    else
    {
        out_color = vec3(1.0, 1.0, 1.0);
    }
}
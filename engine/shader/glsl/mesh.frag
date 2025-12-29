#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"

precision mediump float;

struct DirectionalLight
{
    vec3  direction;
    float _padding_direction;
    vec3  color;
    float _padding_color;
};

struct PointLight
{
    vec3  position;
    float radius;
    vec3  intensity;
    float _padding_intensity;
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
};

layout(set = 1, binding = 0) uniform sampler2D base_color_texture_sampler;
layout(set = 1, binding = 1) uniform sampler2D normal_texture_sampler;

layout(location = 0) in highp vec3 in_world_position;
layout(location = 1) in highp vec3 in_normal;
layout(location = 2) in highp vec3 in_tangent;
layout(location = 3) in highp vec2 in_texcoord;

// read in fragnormal (from vertex shader)
layout(location = 0) out highp vec4 out_scene_color;

highp vec3 calculateNormal()
{
    highp vec3 tangent_normal = texture(normal_texture_sampler, in_texcoord).xyz * 2.0 - 1.0;

    // TBN
    highp vec3 N = normalize(in_normal);
    highp vec3 T = normalize(in_tangent.xyz);
    highp vec3 B = normalize(cross(N, T));

    highp mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangent_normal);
}

#include "mesh_lighting.h"

void main()
{
    highp vec3 L           = normalize(scene_directional_light.direction);
    highp vec3 N           = calculateNormal();
    highp vec3 V           = normalize(camera_position - in_world_position);
    highp vec3 objectColor = texture(base_color_texture_sampler, in_texcoord).xyz;

    highp vec3 result_color = vec3(0.0, 0.0, 0.0);

#include "mesh_lighting.inl"

    out_scene_color = vec4(result_color, 1.0);
}

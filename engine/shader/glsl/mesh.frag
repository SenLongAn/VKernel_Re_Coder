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

layout(set = 0, binding = 0) readonly buffer _unused_name_perframe
{
    mat4             proj_view_matrix;
    vec3             camera_position;
    float            _padding_camera_position;
    vec3             ambient_light;
    float            _padding_ambient_light;
    DirectionalLight scene_directional_light;
};

layout(set = 1, binding = 0) uniform sampler2D base_color_texture_sampler;

layout(location = 0) in highp vec3 in_world_position;
layout(location = 1) in highp vec3 in_normal;
layout(location = 2) in highp vec2 in_texcoord;

// read in fragnormal (from vertex shader)
layout(location = 0) out highp vec4 out_scene_color;

void main()
{
    highp vec3 N         = normalize(in_normal);
    highp vec3 basecolor = texture(base_color_texture_sampler, in_texcoord).xyz;
    highp vec3 result_color;

    // ambient
    highp vec3 ambient = basecolor * ambient_light;

    // diffuse
    highp float diff    = max(dot(N, scene_directional_light.direction), 0.0);
    highp vec3  diffuse = basecolor * diff * scene_directional_light.color * 0.8;

    highp vec3  viewDir    = normalize(camera_position - in_world_position);
    highp vec3  reflectDir = reflect(-scene_directional_light.direction, N);
    highp float spec       = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    highp vec3  specular   = basecolor * spec * scene_directional_light.color * 1.0;

    result_color = ambient + diffuse + specular;

    out_scene_color = vec4(result_color, 1.0);
}

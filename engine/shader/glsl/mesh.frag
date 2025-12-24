#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"

precision mediump float;

layout(set = 1, binding = 0) uniform sampler2D base_color_texture_sampler;

layout(location = 0) in highp vec3 in_world_position;
layout(location = 1) in highp vec3 in_normal;
layout(location = 2) in highp vec2 in_texcoord;

// read in fragnormal (from vertex shader)
layout(location = 0) out highp vec4 out_scene_color;

void main()
{
    // out_scene_color = texture(base_color_texture_sampler, in_texcoord);
    out_scene_color = vec4(in_normal, 1.0);
}

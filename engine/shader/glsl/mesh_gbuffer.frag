#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"
#include "gbuffer.h"

layout(set = 1, binding = 0) uniform sampler2D base_color_texture_sampler;
layout(set = 1, binding = 1) uniform sampler2D normal_texture_sampler;

// from vs
layout(location = 0) in highp vec3 in_world_position;
layout(location = 1) in highp vec3 in_normal;
layout(location = 2) in highp vec3 in_tangent;
layout(location = 3) in highp vec2 in_texcoord;

// output
layout(location = 0) out highp vec4 out_gbuffer_a;
layout(location = 1) out highp vec4 out_gbuffer_b;
layout(location = 2) out highp vec4 out_gbuffer_c;

// calculate Normal
highp vec3 calculateNormal()
{
    highp vec3 tangent_normal = texture(normal_texture_sampler, in_texcoord).xyz * 2.0 - 1.0;

    highp vec3 N = normalize(in_normal);
    highp vec3 T = normalize(in_tangent.xyz);
    highp vec3 B = normalize(cross(N, T));

    highp mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangent_normal);
}

void main()
{
    // calculate and output
    PGBufferData gbuffer;
    gbuffer.worldNormal    = calculateNormal();
    gbuffer.baseColor      = texture(base_color_texture_sampler, in_texcoord).xyz;
    gbuffer.metallic       = 0.5;
    gbuffer.specular       = 0.5;
    gbuffer.roughness      = 0.5;
    gbuffer.shadingModelID = SHADINGMODELID_DEFAULT_LIT;

    EncodeGBufferData(gbuffer, out_gbuffer_a, out_gbuffer_b, out_gbuffer_c);
}
#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"
#include "structures.h"

layout(set = 0, binding = 0) readonly buffer _unused_name_perframe
{
    mat4 proj_view_matrix; ///< pv
};

layout(set = 0, binding = 1) readonly buffer _unused_name_perdrawcall
{
    mat4 model_matrices[m_mesh_per_drawcall_max_instance_count]; ///< M
    uint node_ids[m_mesh_per_drawcall_max_instance_count];       ///< ID
};

layout(location = 0) in vec3 in_position;

layout(location = 0) flat out highp uint out_nodeid;
layout(location = 1) out highp vec4 out_pos;

void main()
{
    // MVP
    highp mat4 model_matrix = model_matrices[gl_InstanceIndex];
    highp vec4 pos          = model_matrix * vec4(in_position, 1.0);
    gl_Position             = proj_view_matrix * pos;

    out_nodeid = node_ids[gl_InstanceIndex];
    out_pos    = pos;
}
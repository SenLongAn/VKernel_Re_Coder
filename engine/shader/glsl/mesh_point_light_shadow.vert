#version 450 core

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"
#include "structures.h"

// M matrix
layout(set = 0, binding = 1) readonly buffer _unused_name_per_drawcall
{
    VulkanMeshInstance mesh_instances[m_mesh_per_drawcall_max_instance_count];
};

// in
layout(location = 0) in highp vec3 in_position;

// out
layout(location = 0) out highp vec3 out_position_world_space;

void main()
{
    // M
    highp mat4 model_matrix  = mesh_instances[gl_InstanceIndex].model_matrix;
    highp vec4 world_pos     = model_matrix * vec4(in_position, 1.0);
    out_position_world_space = world_pos.xyz;
    gl_Position              = world_pos;
}

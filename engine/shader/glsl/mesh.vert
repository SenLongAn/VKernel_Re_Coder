#version 310 es

#extension GL_GOOGLE_include_directive : enable // Enable extension

#include "constants.h"
#include "structures.h"

layout(set = 0, binding = 0) readonly buffer _unused_name_perframe { mat4 proj_view_matrix; };

layout(set = 0, binding = 1) readonly buffer _unused_name_per_drawcall
{
    VulkanMeshInstance mesh_instances[m_mesh_per_drawcall_max_instance_count];
};

layout(location = 0) in vec3 in_position; // for some types as dvec3 takes 2 locations
layout(location = 1) in vec2 in_texcoord;

layout(location = 0) out vec2 out_texcoord;

void main()
{
    highp mat4 model_matrix = mesh_instances[gl_InstanceIndex].model_matrix;
    gl_Position             = proj_view_matrix * model_matrix * vec4(in_position, 1.0);

    out_texcoord = in_texcoord;
}

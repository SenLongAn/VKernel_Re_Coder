#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"
#include "structures.h"

layout(set = 0, binding = 0) readonly buffer _unused_name_global_set_per_frame_binding_buffer // PV
{
    mat4 light_proj_view;
};

layout(set = 0, binding = 1) readonly buffer _unused_name_per_drawcall /// M
{
    VulkanMeshInstance mesh_instances[m_mesh_per_drawcall_max_instance_count];
};

layout(location = 0) in highp vec3 in_position;

void main()
{
    // MVP
    highp mat4 model_matrix = mesh_instances[gl_InstanceIndex].model_matrix;

    highp vec3 position_world_space = (model_matrix * vec4(in_position, 1.0)).xyz;

    gl_Position = light_proj_view * vec4(position_world_space, 1.0f);
}

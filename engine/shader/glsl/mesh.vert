#version 310 es

#extension GL_GOOGLE_include_directive : enable // Enable extension

#include "constants.h"
#include "structures.h"

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

layout(set = 0, binding = 1) readonly buffer _unused_name_per_drawcall
{
    VulkanMeshInstance mesh_instances[m_mesh_per_drawcall_max_instance_count];
};

// in
layout(location = 0) in vec3 in_position; // for some types as dvec3 takes 2 locations
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

// out
layout(location = 0) out vec3 out_world_position;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec2 out_texcoord;

void main()
{
    highp mat4 model_matrix = mesh_instances[gl_InstanceIndex].model_matrix;
    gl_Position             = proj_view_matrix * model_matrix * vec4(in_position, 1.0);

    // out
    out_world_position    = (model_matrix * vec4(in_position, 1.0)).xyz;
    mat3x3 tangent_matrix = mat3x3(model_matrix[0].xyz, model_matrix[1].xyz, model_matrix[2].xyz);
    out_normal            = normalize(tangent_matrix * in_normal);
    out_texcoord          = in_texcoord;
}

#pragma once

#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/vector3.h"

#include "runtime/function/render/render_type.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

/**
 * Render the required data portions
 */
namespace VKernel
{
    // size
    static const uint32_t s_point_light_shadow_map_dimension       = 2048;
    static const uint32_t s_directional_light_shadow_map_dimension = 4096;

    // max count
    static uint32_t const s_mesh_per_drawcall_max_instance_count = 64; ///< instance GO max count
    static uint32_t const s_max_point_light_count                = 15;

    // light
    struct VulkanSceneDirectionalLight ///< Directional Light
    {
        Vector3 direction;          ///< direction
        float   _padding_direction; ///< Memory alignment
        Vector3 color;              ///< color
        float   _padding_color;
    };

    struct VulkanScenePointLight
    {
        Vector3 position;  ///< positino
        float   radius;    ///< radius
        Vector3 intensity; ///< intensity
        float   _padding_intensity;
    };

    struct MeshPointLightShadowPerframeStorageBufferObject
    {
        uint32_t  point_light_num; ///< count
        uint32_t  _padding_point_light_num_1;
        uint32_t  _padding_point_light_num_2;
        uint32_t  _padding_point_light_num_3;
        Vector4   point_lights_position_and_radius[s_max_point_light_count]; ///< position and radius
        Matrix4x4 point_light_matrices[s_max_point_light_count * 6];         ///< VP
    };

    // MVP
    struct MeshPerframeStorageBufferObject ///< Read-only
    {
        Matrix4x4 proj_view_matrix; ///< pv

        Vector3 camera_position; ///< camera position
        float   _padding_camera_position;

        Vector3 ambient_light; ///< ambient light
        float   _padding_ambient_light;

        uint32_t              point_light_num; ///< point num
        uint32_t              _padding_point_light_num_1;
        uint32_t              _padding_point_light_num_2;
        uint32_t              _padding_point_light_num_3;
        VulkanScenePointLight scene_point_lights[s_max_point_light_count];       ///< point lights
        Matrix4x4             point_light_matrices[s_max_point_light_count * 6]; ///< VP

        VulkanSceneDirectionalLight scene_directional_light; ///< Directional Light
        Matrix4x4                   directional_light_proj_view;
    };

    struct MeshDirectionalLightShadowPerframeStorageBufferObject
    {
        Matrix4x4 light_proj_view;
    };

    struct VulkanMeshInstance ///< Instantiate the properties of each object
    {
        Matrix4x4 model_matrix; ///< M
    };

    struct MeshDirectionalLightShadowPerdrawcallStorageBufferObject
    {
        VulkanMeshInstance mesh_instances[s_mesh_per_drawcall_max_instance_count];
    };

    struct MeshPerdrawcallStorageBufferObject
    {
        VulkanMeshInstance mesh_instances[s_mesh_per_drawcall_max_instance_count];
    };

    struct MeshPointLightShadowPerdrawcallStorageBufferObject
    {
        VulkanMeshInstance mesh_instances[s_mesh_per_drawcall_max_instance_count];
    };

    struct MeshInefficientPickPerframeStorageBufferObject
    {
        Matrix4x4 proj_view_matrix;
    };

    struct MeshInefficientPickPerdrawcallStorageBufferObject
    {
        Matrix4x4 model_matrices[s_mesh_per_drawcall_max_instance_count]; ///< M matrix
        uint32_t  node_ids[s_mesh_per_drawcall_max_instance_count];       ///< submesh id
    };

    struct AxisStorageBufferObject
    {
        Matrix4x4 model_matrix  = Matrix4x4::IDENTITY;
        uint32_t  selected_axis = 3;
    };

    // mesh
    struct VulkanMesh
    {
        // vertex
        uint32_t mesh_vertex_count; ///< count

        // render_mesh.h
        VkBuffer      mesh_vertex_position_buffer;            ///< buffer
        VmaAllocation mesh_vertex_position_buffer_allocation; ///< allocation

        VkBuffer      mesh_vertex_varying_enable_blending_buffer;
        VmaAllocation mesh_vertex_varying_enable_blending_buffer_allocation;

        VkBuffer      mesh_vertex_varying_buffer;
        VmaAllocation mesh_vertex_varying_buffer_allocation;

        // index
        uint32_t      mesh_index_count;
        VkBuffer      mesh_index_buffer;
        VmaAllocation mesh_index_buffer_allocation;
    };

    // material
    struct VulkanPBRMaterial
    {
        VkImage       base_color_texture_image;    ///< image
        VkImageView   base_color_image_view;       ///< image view
        VmaAllocation base_color_image_allocation; ///< allocation

        VkImage       normal_texture_image;
        VkImageView   normal_image_view;
        VmaAllocation normal_image_allocation;

        VkDescriptorSet material_descriptor_set; ///< descriptor set
    };

    struct TextureDataToUpdate
    {
        void*    base_color_image_pixels; ///< data
        uint32_t base_color_image_width;  ///< width
        uint32_t base_color_image_height; ///< height
        VkFormat base_color_image_format; ///< format

        void*    normal_roughness_image_pixels;
        uint32_t normal_roughness_image_width;
        uint32_t normal_roughness_image_height;
        VkFormat normal_roughness_image_format;

        VulkanPBRMaterial* now_material; ///< image view and descriptor
    };

    struct RenderMeshNode
    {
        uint32_t           node_id;
        const Matrix4x4*   model_matrix {nullptr};
        VulkanMesh*        ref_mesh {nullptr};
        VulkanPBRMaterial* ref_material {nullptr};
    };

    struct RenderAxisNode ///< axis data
    {
        Matrix4x4   model_matrix {Matrix4x4::IDENTITY};
        VulkanMesh* ref_mesh {nullptr};
        uint32_t    node_id;
    };
} // namespace VKernel
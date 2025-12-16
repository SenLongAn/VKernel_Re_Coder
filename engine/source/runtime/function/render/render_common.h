#pragma once

#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/vector3.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

/**
 * Render the required data portions
 */
namespace VKernel
{

    static uint32_t const s_mesh_per_drawcall_max_instance_count = 64; ///< instance GO max count

    struct MeshPerframeStorageBufferObject ///< Read-only
    {
        Matrix4x4 proj_view_matrix;
    };

    struct VulkanMeshInstance ///< Instantiate the properties of each object
    {
        Matrix4x4 model_matrix;
    };

    struct MeshPerdrawcallStorageBufferObject
    {
        VulkanMeshInstance mesh_instances[s_mesh_per_drawcall_max_instance_count];
    };

    // mesh
    struct VulkanMesh
    {
        // vertex
        uint32_t mesh_vertex_count; ///< count

        VkBuffer      mesh_vertex_position_buffer;            ///< buffer
        VmaAllocation mesh_vertex_position_buffer_allocation; ///< allocation

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

        VkDescriptorSet material_descriptor_set; ///< descriptor set
    };

    struct TextureDataToUpdate
    {
        void*    base_color_image_pixels; ///< data
        uint32_t base_color_image_width;  ///< width
        uint32_t base_color_image_height; ///< height
        VkFormat base_color_image_format; ///< format

        VulkanPBRMaterial* now_material; ///< image view and descriptor
    };

    struct RenderMeshNode
    {
        const Matrix4x4*   model_matrix {nullptr};
        VulkanMesh*        ref_mesh {nullptr};
        VulkanPBRMaterial* ref_material {nullptr};
    };
} // namespace VKernel
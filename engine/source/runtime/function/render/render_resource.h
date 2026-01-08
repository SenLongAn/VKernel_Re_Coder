#pragma once

#include "runtime/function/render/render_common.h"
#include "runtime/function/render/render_resource_base.h"

#include <map>
#include <memory>
#include <vector>

/**
 * All the data required for rendering
 */
namespace VKernel
{
    class RenderCamera;
    class RenderPassBase;

    struct IBLResource
    {
        VkImage       _brdfLUT_texture_image;
        VkImageView   _brdfLUT_texture_image_view;
        VkSampler     _brdfLUT_texture_sampler;
        VmaAllocation _brdfLUT_texture_image_allocation;

        VkImage       _irradiance_texture_image;
        VkImageView   _irradiance_texture_image_view;
        VkSampler     _irradiance_texture_sampler;
        VmaAllocation _irradiance_texture_image_allocation;

        VkImage       _specular_texture_image;            ///< image
        VkImageView   _specular_texture_image_view;       ///< image view
        VkSampler     _specular_texture_sampler;          ///< sampler
        VmaAllocation _specular_texture_image_allocation; ///< allocation
    };

    struct StorageBuffer
    {
        uint32_t _min_storage_buffer_offset_alignment {256}; ///< alignment

        uint32_t _max_storage_buffer_range {1 << 27}; ///< max range

        VkBuffer       _global_upload_ringbuffer; ///< buffer, ring: Multiple sets of data placed in the same buffer
        VkDeviceMemory _global_upload_ringbuffer_memory; ///< memory

        void* _global_upload_ringbuffer_memory_pointer; ///< gpu map memory, We will write data here.

        // The end position of a data set , vector is because of synchronization primitives
        std::vector<uint32_t> _global_upload_ringbuffers_begin;
        std::vector<uint32_t> _global_upload_ringbuffers_end;
        std::vector<uint32_t> _global_upload_ringbuffers_size;

        // axis
        VkBuffer       _axis_inefficient_storage_buffer;
        VkDeviceMemory _axis_inefficient_storage_buffer_memory;
        void*          _axis_inefficient_storage_buffer_memory_pointer;
    };

    struct GlobalRenderResource
    {
        IBLResource   _ibl_resource;
        StorageBuffer _storage_buffer;
    };

    class RenderResource : public RenderResourceBase
    {
    public:
        std::map<size_t, VulkanMesh>        m_vulkan_meshes;        ///< mesh id, mesh buffer
        std::map<size_t, VulkanPBRMaterial> m_vulkan_pbr_materials; ///< material id, material image view

        MeshPerframeStorageBufferObject m_mesh_perframe_storage_buffer_object; ///< Render Resource
        MeshDirectionalLightShadowPerframeStorageBufferObject
                                                       m_mesh_directional_light_shadow_perframe_storage_buffer_object;
        MeshInefficientPickPerframeStorageBufferObject m_mesh_inefficient_pick_perframe_storage_buffer_object;

        GlobalRenderResource m_global_render_resource; ///<  Storage buffer

        VkDescriptorSetLayout m_material_descriptor_set_layout; ///< descriptor layout

    public:
        void clear() override final;

        virtual void updatePerFrameBuffer(
            std::shared_ptr<RenderScene>  render_scene,
            std::shared_ptr<RenderCamera> camera) override final; ///< update MeshPerframeStorageBufferObject data

        // create buffer
        virtual void
        uploadGlobalRenderResource(std::shared_ptr<VulkanAPI> vulkan_api,
                                   LevelResourceDesc          level_resource_desc) override final; ///<  Storage buffer

        virtual void
        uploadGameObjectRenderResource(std::shared_ptr<VulkanAPI> vulkan_api,
                                       RenderEntity               render_entity,
                                       RenderMeshData mesh_data) override final; ///< vertex and indice buffer

        virtual void
        uploadGameObjectRenderResource(std::shared_ptr<VulkanAPI> vulkan_api,
                                       RenderEntity               render_entity,
                                       RenderMaterialData material_data) override final; ///< image view and descriptor

        // reset Ring Buffer Offset
        void resetRingBufferOffset(uint8_t current_frame_index);

        // get
        VulkanMesh&        getEntityMesh(RenderEntity entity);
        VulkanPBRMaterial& getEntityMaterial(RenderEntity entity);

    private:
        // create buffer and image
        void createAndMapStorageBuffer(std::shared_ptr<VulkanAPI> vulkan_api); ///< Storage buffer

        VulkanMesh& getOrCreateVulkanMesh(std::shared_ptr<VulkanAPI> vulkan_api,
                                          RenderEntity               entity,
                                          RenderMeshData             mesh_data); ///< vertex and indice

        VulkanPBRMaterial& getOrCreateVulkanMaterial(std::shared_ptr<VulkanAPI> vulkan_api,
                                                     RenderEntity               entity,
                                                     RenderMaterialData material_data); ///< image view and descriptor

        void updateMeshData(std::shared_ptr<VulkanAPI>             vulkan_api,
                            uint32_t                               index_buffer_size,
                            void*                                  index_buffer_data,
                            uint32_t                               vertex_buffer_size,
                            struct MeshVertexDataDefinition const* vertex_buffer_data,
                            VulkanMesh&                            now_mesh); ///<  vertex and indice buffer

        void updateVertexBuffer(std::shared_ptr<VulkanAPI>             vulkan_api,
                                uint32_t                               vertex_buffer_size,
                                struct MeshVertexDataDefinition const* vertex_buffer_data,
                                uint32_t                               index_buffer_size,
                                uint32_t*                              index_buffer_data,
                                VulkanMesh&                            now_mesh); ///< vertex buffer

        void updateIndexBuffer(std::shared_ptr<VulkanAPI> vulkan_api,
                               uint32_t                   index_buffer_size,
                               void*                      index_buffer_data,
                               VulkanMesh&                now_mesh); ///< indice buffer

        void updateTextureImageData(std::shared_ptr<VulkanAPI> vulkan_api,
                                    const TextureDataToUpdate& texture_data); ///< image view

        // create sampler
        void createIBLSamplers(std::shared_ptr<VulkanAPI> vulkan_api);
        void createIBLTextures(std::shared_ptr<VulkanAPI>                  vulkan_api,
                               std::array<std::shared_ptr<TextureData>, 6> irradiance_maps,
                               std::array<std::shared_ptr<TextureData>, 6> specular_maps);
    };
} // namespace VKernel
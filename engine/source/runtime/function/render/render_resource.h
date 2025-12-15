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
    };

    struct GlobalRenderResource
    {
        StorageBuffer _storage_buffer;
    };

    class RenderResource : public RenderResourceBase
    {
    public:
        std::map<size_t, VulkanMesh> m_vulkan_meshes; ///< mesh id, mesh buffer

        MeshPerframeStorageBufferObject m_mesh_perframe_storage_buffer_object; ///< Render Resource

        GlobalRenderResource m_global_render_resource;

    public:
        void clear() override final;

        virtual void updatePerFrameBuffer(std::shared_ptr<RenderCamera> camera) override final;

        virtual void uploadGlobalRenderResource(std::shared_ptr<VulkanAPI> vulkan_api) override final;

        // create buffer and descriptor
        virtual void uploadGameObjectRenderResource(std::shared_ptr<VulkanAPI> vulkan_api,
                                                    RenderEntity               render_entity,
                                                    RenderMeshData             mesh_data) override final;

        void resetRingBufferOffset(uint8_t current_frame_index); // reset Ring Buffer Offset

        // get
        VulkanMesh& getEntityMesh(RenderEntity entity);

    private:
        // create buffer and descriptor
        void createAndMapStorageBuffer(std::shared_ptr<VulkanAPI> vulkan_api); ///< Storage

        VulkanMesh& getOrCreateVulkanMesh(std::shared_ptr<VulkanAPI> vulkan_api,
                                          RenderEntity               entity,
                                          RenderMeshData             mesh_data); ///< vertex and indice

        void updateMeshData(std::shared_ptr<VulkanAPI>             vulkan_api,
                            uint32_t                               index_buffer_size,
                            void*                                  index_buffer_data,
                            uint32_t                               vertex_buffer_size,
                            struct MeshVertexDataDefinition const* vertex_buffer_data,
                            VulkanMesh&                            now_mesh);

        void updateVertexBuffer(std::shared_ptr<VulkanAPI>             vulkan_api,
                                uint32_t                               vertex_buffer_size,
                                struct MeshVertexDataDefinition const* vertex_buffer_data,
                                uint32_t                               index_buffer_size,
                                uint32_t*                              index_buffer_data,
                                VulkanMesh&                            now_mesh);

        void updateIndexBuffer(std::shared_ptr<VulkanAPI> vulkan_api,
                               uint32_t                   index_buffer_size,
                               void*                      index_buffer_data,
                               VulkanMesh&                now_mesh);
    };
} // namespace VKernel
#include "runtime/function/render/render_resource.h"

#include "render_resource.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_mesh.h"

namespace VKernel
{
    void RenderResource::clear() {}

    void RenderResource::updatePerFrameBuffer(std::shared_ptr<RenderCamera> camera)
    {
        // Get proj view matrix from the camera
        Matrix4x4 view_matrix      = camera->getViewMatrix();
        Matrix4x4 proj_matrix      = camera->getPersProjMatrix();
        Matrix4x4 proj_view_matrix = proj_matrix * view_matrix;

        m_mesh_perframe_storage_buffer_object.proj_view_matrix = proj_view_matrix;
    }

    void RenderResource::uploadGlobalRenderResource(std::shared_ptr<VulkanAPI> vulkan_api)
    {
        // create and map global storage buffer
        createAndMapStorageBuffer(vulkan_api);
    }

    void RenderResource::uploadGameObjectRenderResource(std::shared_ptr<VulkanAPI> vulkan_api, RenderMeshData mesh_data)
    {
        getOrCreateVulkanMesh(vulkan_api, mesh_data);
    }

    VulkanMesh& RenderResource::getEntityMesh(size_t id)
    {
        auto it = m_vulkan_meshes.find(id);
        if (it != m_vulkan_meshes.end()) ///< If found, return the mesh
        {
            return it->second;
        }
        else ///< else throw
        {
            throw std::runtime_error("failed to get entity mesh");
        }
    }

    void RenderResource::createAndMapStorageBuffer(std::shared_ptr<VulkanAPI> vulkan_api)
    {
        // createBuffer
        StorageBuffer& _storage_buffer = m_global_render_resource._storage_buffer;

        uint32_t global_storage_buffer_size = 1024 * 1024 * 128;
        vulkan_api->createBuffer(global_storage_buffer_size,
                                 VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                 _storage_buffer._global_upload_ringbuffer,
                                 _storage_buffer._global_upload_ringbuffer_memory);

        // Unmap when program terminates
        vkMapMemory(vulkan_api->getLogicDevice(),
                    _storage_buffer._global_upload_ringbuffer_memory,
                    0,
                    VK_WHOLE_SIZE,
                    0,
                    &_storage_buffer._global_upload_ringbuffer_memory_pointer);
    }

    VulkanMesh& RenderResource::getOrCreateVulkanMesh(std::shared_ptr<VulkanAPI> vulkan_api, RenderMeshData mesh_data)
    {
        // insert nullptr data
        VulkanMesh temp;
        auto       res = m_vulkan_meshes.insert(std::make_pair(0, std::move(temp)));
        assert(res.second);

        // get index and vertex data
        uint32_t index_index_buffer_size  = static_cast<uint32_t>(mesh_data.m_static_mesh_data.m_index_buffer->m_size);
        void*    index_buffer_data        = mesh_data.m_static_mesh_data.m_index_buffer->m_data;
        uint32_t vertex_index_buffer_size = static_cast<uint32_t>(mesh_data.m_static_mesh_data.m_vertex_buffer->m_size);
        MeshVertexDataDefinition* vertex_buffer_data = reinterpret_cast<MeshVertexDataDefinition*>(
            mesh_data.m_static_mesh_data.m_vertex_buffer->m_data); ///< Type cast to MeshVertexDataDefinition

        // create buffer and descriptor
        VulkanMesh& now_mesh = res.first->second; ///< Return the second item of the inserted data

        updateMeshData(vulkan_api,
                       index_index_buffer_size,
                       index_buffer_data,
                       vertex_index_buffer_size,
                       vertex_buffer_data,
                       now_mesh);

        return now_mesh;
    }

    void RenderResource::updateMeshData(std::shared_ptr<VulkanAPI>      vulkan_api,
                                        uint32_t                        index_index_buffer_size,
                                        void*                           index_buffer_data,
                                        uint32_t                        vertex_index_buffer_size,
                                        MeshVertexDataDefinition const* vertex_buffer_data,
                                        VulkanMesh&                     now_mesh)
    {
        // vertex
        assert(0 == (vertex_index_buffer_size % sizeof(MeshVertexDataDefinition))); ///< Is it an integer multiple
        now_mesh.mesh_vertex_count =
            vertex_index_buffer_size / sizeof(MeshVertexDataDefinition); ///< Calculate the number of meshe vertex
        updateVertexBuffer(vulkan_api,
                           vertex_index_buffer_size,
                           vertex_buffer_data,
                           index_index_buffer_size,
                           reinterpret_cast<uint32_t*>(index_buffer_data),
                           now_mesh);

        // indices
        assert(0 == (index_index_buffer_size % sizeof(uint32_t)));              ///< Is it an integer multiple
        now_mesh.mesh_index_count = index_index_buffer_size / sizeof(uint32_t); ///< Calculate the number of indices
        updateIndexBuffer(vulkan_api, index_index_buffer_size, index_buffer_data, now_mesh);
    }

    void RenderResource::updateVertexBuffer(std::shared_ptr<VulkanAPI>      vulkan_api,
                                            uint32_t                        vertex_index_buffer_size,
                                            MeshVertexDataDefinition const* vertex_buffer_data,
                                            uint32_t                        index_index_buffer_size,
                                            uint32_t*                       index_buffer_data,
                                            VulkanMesh&                     now_mesh)
    {
        // Calculate count
        assert(0 == (vertex_index_buffer_size % sizeof(MeshVertexDataDefinition))); ///< Is it an integer multiple
        uint32_t vertex_count =
            vertex_index_buffer_size / sizeof(MeshVertexDataDefinition); ///< Calculate the number of meshe vertex

        // Calculate offset
        VkDeviceSize vertex_position_index_buffer_size = sizeof(MeshVertex::VulkanMeshVertexPostition) * vertex_count;
        VkDeviceSize vertex_position_buffer_offset     = 0;

        // create temporary staging buffer
        VkDeviceSize   inefficient_staging_index_buffer_size = vertex_position_index_buffer_size; ///< size
        VkBuffer       inefficient_staging_buffer            = VK_NULL_HANDLE;                    ///< buffer
        VkDeviceMemory inefficient_staging_buffer_memory     = VK_NULL_HANDLE;                    ///< memory
        vulkan_api->createBuffer(inefficient_staging_index_buffer_size,
                                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                 inefficient_staging_buffer,
                                 inefficient_staging_buffer_memory);

        // Write data to the buffer
        void* inefficient_staging_buffer_data;
        vkMapMemory(vulkan_api->getLogicDevice(),
                    inefficient_staging_buffer_memory,
                    0,
                    VK_WHOLE_SIZE,
                    0,
                    &inefficient_staging_buffer_data);

        MeshVertex::VulkanMeshVertexPostition* mesh_vertex_positions = ///< The starting position of data in MapMemory =
                                                                       ///< MapMemory starting address + data offset
            reinterpret_cast<MeshVertex::VulkanMeshVertexPostition*>( ///< Convert a pointer to an integer to obtain the
                                                                      ///< memory address value of the pointer
                reinterpret_cast<uintptr_t>(inefficient_staging_buffer_data) +
                vertex_position_buffer_offset); ///< Conversion between pointers and integers

        for (uint32_t vertex_index = 0; vertex_index < vertex_count; ++vertex_index) ///< Iterative Vertex
        {
            // Manipulating mesh_vertex_positions is essentially manipulating inefficient_staging_buffer_data.
            mesh_vertex_positions[vertex_index].position = Vector3(vertex_buffer_data[vertex_index].x,
                                                                   vertex_buffer_data[vertex_index].y,
                                                                   vertex_buffer_data[vertex_index].z);
        }

        vkUnmapMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory);

        // use the vmaAllocator to allocate asset vertex buffer
        VkBufferCreateInfo bufferInfo = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferInfo.usage              = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage                   = VMA_MEMORY_USAGE_GPU_ONLY;

        bufferInfo.size = vertex_position_index_buffer_size;
        vmaCreateBuffer(vulkan_api->getVmaAllocator(),
                        &bufferInfo,
                        &allocInfo,
                        &now_mesh.mesh_vertex_position_buffer,
                        &now_mesh.mesh_vertex_position_buffer_allocation,
                        NULL);

        // copy data from staging buffer
        vulkan_api->copyBuffer(inefficient_staging_buffer,
                               now_mesh.mesh_vertex_position_buffer,
                               vertex_position_buffer_offset,
                               0,
                               vertex_position_index_buffer_size);

        // release staging buffer
        vkDestroyBuffer(vulkan_api->getLogicDevice(), inefficient_staging_buffer, nullptr);
        vkFreeMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory, nullptr);
    }

    void RenderResource::updateIndexBuffer(std::shared_ptr<VulkanAPI> vulkan_api,
                                           uint32_t                   index_buffer_size,
                                           void*                      index_buffer_data,
                                           VulkanMesh&                now_mesh)
    {
        // temp staging buffer
        VkBuffer       inefficient_staging_buffer;
        VkDeviceMemory inefficient_staging_buffer_memory;
        vulkan_api->createBuffer(index_buffer_size,
                                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                 inefficient_staging_buffer,
                                 inefficient_staging_buffer_memory);

        // Write data to the buffer
        void* staging_buffer_data;
        vkMapMemory(vulkan_api->getLogicDevice(),
                    inefficient_staging_buffer_memory,
                    0,
                    index_buffer_size,
                    0,
                    &staging_buffer_data);
        memcpy(staging_buffer_data, index_buffer_data, (size_t)index_buffer_size);
        vkUnmapMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory);

        // use the vmaAllocator to allocate asset index buffer
        VkBufferCreateInfo bufferInfo = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferInfo.size               = index_buffer_size;
        bufferInfo.usage              = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage                   = VMA_MEMORY_USAGE_GPU_ONLY;

        vmaCreateBuffer(vulkan_api->getVmaAllocator(),
                        &bufferInfo,
                        &allocInfo,
                        &now_mesh.mesh_index_buffer,
                        &now_mesh.mesh_index_buffer_allocation,
                        NULL);

        // copy data from staging buffer
        vulkan_api->copyBuffer(inefficient_staging_buffer, now_mesh.mesh_index_buffer, 0, 0, index_buffer_size);

        // release temp staging buffer
        vkDestroyBuffer(vulkan_api->getLogicDevice(), inefficient_staging_buffer, nullptr);
        vkFreeMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory, nullptr);
    }
} // namespace VKernel
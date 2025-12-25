#include "runtime/function/render/render_resource.h"

#include "render_resource.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_mesh.h"
#include "runtime/function/render/render_scene.h"

namespace VKernel
{
    void RenderResource::clear() {}

    void RenderResource::updatePerFrameBuffer(std::shared_ptr<RenderScene>  render_scene,
                                              std::shared_ptr<RenderCamera> camera)
    {
        // Get data
        Vector3   camera_position  = camera->position();
        Matrix4x4 view_matrix      = camera->getViewMatrix();
        Matrix4x4 proj_matrix      = camera->getPersProjMatrix();
        Matrix4x4 proj_view_matrix = proj_matrix * view_matrix;
        Vector3   ambient_light    = render_scene->m_ambient_light.m_irradiance;

        // set data
        m_mesh_perframe_storage_buffer_object.proj_view_matrix = proj_view_matrix;
        m_mesh_perframe_storage_buffer_object.camera_position  = camera_position;
        m_mesh_perframe_storage_buffer_object.ambient_light    = ambient_light;
        m_mesh_perframe_storage_buffer_object.scene_directional_light.direction =
            render_scene->m_directional_light.m_direction.normalisedCopy();
        m_mesh_perframe_storage_buffer_object.scene_directional_light.color = render_scene->m_directional_light.m_color;
    }

    void RenderResource::uploadGlobalRenderResource(std::shared_ptr<VulkanAPI> vulkan_api)
    {
        // create and map global storage buffer
        createAndMapStorageBuffer(vulkan_api);
    }

    void RenderResource::uploadGameObjectRenderResource(std::shared_ptr<VulkanAPI> vulkan_api,
                                                        RenderEntity               render_entity,
                                                        RenderMeshData             mesh_data)
    {
        getOrCreateVulkanMesh(vulkan_api, render_entity, mesh_data);
    }

    void RenderResource::uploadGameObjectRenderResource(std::shared_ptr<VulkanAPI> vulkan_api,
                                                        RenderEntity               render_entity,
                                                        RenderMaterialData         material_data)
    {
        getOrCreateVulkanMaterial(vulkan_api, render_entity, material_data);
    }

    void RenderResource::resetRingBufferOffset(uint8_t current_frame_index)
    {
        m_global_render_resource._storage_buffer._global_upload_ringbuffers_end[current_frame_index] =
            m_global_render_resource._storage_buffer
                ._global_upload_ringbuffers_begin[current_frame_index]; ///< end offset to start offset
    }

    VulkanMesh& RenderResource::getEntityMesh(RenderEntity entity)
    {
        size_t assetid = entity.m_mesh_id;
        auto   it      = m_vulkan_meshes.find(assetid);
        if (it != m_vulkan_meshes.end()) ///< If found, return the mesh
        {
            return it->second;
        }
        else ///< else throw
        {
            throw std::runtime_error("failed to get entity mesh");
        }
    }

    VulkanPBRMaterial& RenderResource::getEntityMaterial(RenderEntity entity)
    {
        size_t assetid = entity.m_material_asset_id;

        auto it = m_vulkan_pbr_materials.find(assetid);
        if (it != m_vulkan_pbr_materials.end())
        {
            return it->second;
        }
        else
        {
            throw std::runtime_error("failed to get entity material");
        }
    }

    void RenderResource::createAndMapStorageBuffer(std::shared_ptr<VulkanAPI> vulkan_api)
    {
        StorageBuffer& _storage_buffer  = m_global_render_resource._storage_buffer;
        uint32_t       frames_in_flight = vulkan_api->getMaxFramesInFlight();

        // createBuffer
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

        // init storage buffer offset
        _storage_buffer._global_upload_ringbuffers_begin.resize(frames_in_flight);
        _storage_buffer._global_upload_ringbuffers_end.resize(frames_in_flight);
        _storage_buffer._global_upload_ringbuffers_size.resize(frames_in_flight);

        for (uint32_t i = 0; i < frames_in_flight; ++i)
        {
            _storage_buffer._global_upload_ringbuffers_begin[i] =
                (global_storage_buffer_size * i) /
                frames_in_flight; ///< Divide the global_storage_buffer_size into starting offsets for frames_in_flight
            _storage_buffer._global_upload_ringbuffers_size[i] =
                (global_storage_buffer_size * (i + 1)) / frames_in_flight -
                (global_storage_buffer_size * i) / frames_in_flight; ///< Serving size
        }
    }

    VulkanMesh& RenderResource::getOrCreateVulkanMesh(std::shared_ptr<VulkanAPI> vulkan_api,
                                                      RenderEntity               entity,
                                                      RenderMeshData             mesh_data)
    {
        // Check if it has been added
        size_t assetid = entity.m_mesh_id;
        auto   it      = m_vulkan_meshes.find(assetid);
        if (it != m_vulkan_meshes.end())
        {
            return it->second;
        }
        else
        {
            // insert nullptr data
            VulkanMesh temp;
            auto       res = m_vulkan_meshes.insert(std::make_pair(assetid, std::move(temp)));
            assert(res.second);

            // get index and vertex data
            uint32_t index_buffer_size  = static_cast<uint32_t>(mesh_data.m_static_mesh_data.m_index_buffer->m_size);
            void*    index_buffer_data  = mesh_data.m_static_mesh_data.m_index_buffer->m_data;
            uint32_t vertex_buffer_size = static_cast<uint32_t>(mesh_data.m_static_mesh_data.m_vertex_buffer->m_size);
            MeshVertexDataDefinition* vertex_buffer_data = reinterpret_cast<MeshVertexDataDefinition*>(
                mesh_data.m_static_mesh_data.m_vertex_buffer->m_data); ///< Type cast to MeshVertexDataDefinition

            // create buffer and descriptor
            VulkanMesh& now_mesh = res.first->second; ///< Return the second item of the inserted data

            updateMeshData(
                vulkan_api, index_buffer_size, index_buffer_data, vertex_buffer_size, vertex_buffer_data, now_mesh);

            return now_mesh;
        }
    }

    VulkanPBRMaterial& RenderResource::getOrCreateVulkanMaterial(std::shared_ptr<VulkanAPI> vulkan_api,
                                                                 RenderEntity               entity,
                                                                 RenderMaterialData         material_data)
    {
        // Check if it has been added
        size_t assetid = entity.m_material_asset_id;

        auto it = m_vulkan_pbr_materials.find(assetid);
        if (it != m_vulkan_pbr_materials.end())
        {
            return it->second;
        }
        else
        {
            // insert nullptr data
            VulkanPBRMaterial temp;
            auto              res = m_vulkan_pbr_materials.insert(std::make_pair(assetid, std::move(temp)));
            assert(res.second);

            // write data
            float    empty_image[]           = {0.5f, 0.5f, 0.5f, 0.5f};
            void*    base_color_image_pixels = empty_image;
            uint32_t base_color_image_width  = 1;
            uint32_t base_color_image_height = 1;
            VkFormat base_color_image_format = VkFormat::VK_FORMAT_R8G8B8A8_SRGB;
            if (material_data.m_base_color_texture)
            {
                base_color_image_pixels = material_data.m_base_color_texture->m_pixels;
                base_color_image_width  = static_cast<uint32_t>(material_data.m_base_color_texture->m_width);
                base_color_image_height = static_cast<uint32_t>(material_data.m_base_color_texture->m_height);
                base_color_image_format = material_data.m_base_color_texture->m_format;
            }

            // create image and image view
            VulkanPBRMaterial& now_material = res.first->second; ///< Return the second item of the inserted data

            TextureDataToUpdate update_texture_data; ///< write data
            update_texture_data.base_color_image_pixels = base_color_image_pixels;
            update_texture_data.base_color_image_width  = base_color_image_width;
            update_texture_data.base_color_image_height = base_color_image_height;
            update_texture_data.base_color_image_format = base_color_image_format;
            update_texture_data.now_material            = &now_material;

            updateTextureImageData(vulkan_api, update_texture_data);

            // allocate descriptor
            VkDescriptorSetAllocateInfo material_descriptor_set_alloc_info;
            material_descriptor_set_alloc_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            material_descriptor_set_alloc_info.pNext              = NULL;
            material_descriptor_set_alloc_info.descriptorPool     = vulkan_api->getDescriptorPool();
            material_descriptor_set_alloc_info.descriptorSetCount = 1;
            material_descriptor_set_alloc_info.pSetLayouts        = &m_material_descriptor_set_layout;

            if (VK_SUCCESS != vkAllocateDescriptorSets(vulkan_api->getLogicDevice(),
                                                       &material_descriptor_set_alloc_info,
                                                       &now_material.material_descriptor_set))
            {
                throw std::runtime_error("allocate material descriptor set");
            }

            // bind descriptor and image view
            VkDescriptorImageInfo base_color_image_info = {};
            base_color_image_info.imageLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; ///< only read
            base_color_image_info.imageView             = now_material.base_color_image_view;
            base_color_image_info.sampler =
                vulkan_api->getOrCreateMipmapSampler(base_color_image_width, base_color_image_height);

            VkWriteDescriptorSet mesh_descriptor_writes_info[1];

            mesh_descriptor_writes_info[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            mesh_descriptor_writes_info[0].pNext           = NULL;
            mesh_descriptor_writes_info[0].dstSet          = now_material.material_descriptor_set;
            mesh_descriptor_writes_info[0].dstBinding      = 0;
            mesh_descriptor_writes_info[0].dstArrayElement = 0;
            mesh_descriptor_writes_info[0].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            mesh_descriptor_writes_info[0].descriptorCount = 1;
            mesh_descriptor_writes_info[0].pImageInfo      = &base_color_image_info;

            vkUpdateDescriptorSets(vulkan_api->getLogicDevice(), 1, mesh_descriptor_writes_info, 0, nullptr);
        }
    }

    void RenderResource::updateMeshData(std::shared_ptr<VulkanAPI>      vulkan_api,
                                        uint32_t                        index_buffer_size,
                                        void*                           index_buffer_data,
                                        uint32_t                        vertex_buffer_size,
                                        MeshVertexDataDefinition const* vertex_buffer_data,
                                        VulkanMesh&                     now_mesh)
    {
        // vertex
        assert(0 == (vertex_buffer_size % sizeof(MeshVertexDataDefinition))); ///< Is it an integer multiple
        now_mesh.mesh_vertex_count =
            vertex_buffer_size / sizeof(MeshVertexDataDefinition); ///< Calculate the number of meshe vertex
        updateVertexBuffer(vulkan_api,
                           vertex_buffer_size,
                           vertex_buffer_data,
                           index_buffer_size,
                           reinterpret_cast<uint32_t*>(index_buffer_data),
                           now_mesh);

        // indices
        assert(0 == (index_buffer_size % sizeof(uint32_t)));              ///< Is it an integer multiple
        now_mesh.mesh_index_count = index_buffer_size / sizeof(uint32_t); ///< Calculate the number of indices
        updateIndexBuffer(vulkan_api, index_buffer_size, index_buffer_data, now_mesh);
    }

    void RenderResource::updateVertexBuffer(std::shared_ptr<VulkanAPI>      vulkan_api,
                                            uint32_t                        vertex_buffer_size,
                                            MeshVertexDataDefinition const* vertex_buffer_data,
                                            uint32_t                        index_buffer_size,
                                            uint32_t*                       index_buffer_data,
                                            VulkanMesh&                     now_mesh)
    {
        // Calculate count
        assert(0 == (vertex_buffer_size % sizeof(MeshVertexDataDefinition))); ///< Is it an integer multiple
        uint32_t vertex_count =
            vertex_buffer_size / sizeof(MeshVertexDataDefinition); ///< Calculate the number of meshe vertex

        // Calculate offset
        VkDeviceSize vertex_position_buffer_size =
            sizeof(MeshVertex::VulkanMeshVertexPostition) * vertex_count; ///< size
        VkDeviceSize vertex_varying_enable_blending_buffer_size =
            sizeof(MeshVertex::VulkanMeshVertexVaryingEnableBlending) * vertex_count;
        VkDeviceSize vertex_varying_buffer_size = sizeof(MeshVertex::VulkanMeshVertexVarying) * vertex_count;

        VkDeviceSize vertex_position_buffer_offset = 0; ///< offset
        VkDeviceSize vertex_varying_enable_blending_buffer_offset =
            vertex_position_buffer_offset + vertex_position_buffer_size;
        VkDeviceSize vertex_varying_buffer_offset =
            vertex_varying_enable_blending_buffer_offset + vertex_varying_enable_blending_buffer_size;

        // create temporary staging buffer
        VkDeviceSize inefficient_staging_index_buffer_size = vertex_position_buffer_size +
                                                             vertex_varying_enable_blending_buffer_size +
                                                             vertex_varying_buffer_size; ///< size
        VkBuffer       inefficient_staging_buffer        = VK_NULL_HANDLE;               ///< buffer
        VkDeviceMemory inefficient_staging_buffer_memory = VK_NULL_HANDLE;               ///< memory
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
            reinterpret_cast<MeshVertex::VulkanMeshVertexPostition*>(  ///< Convert a pointer to an integer to obtain
                                                                       ///< the memory address value of the pointer
                reinterpret_cast<uintptr_t>(inefficient_staging_buffer_data) +
                vertex_position_buffer_offset); ///< Conversion between pointers and integers

        MeshVertex::VulkanMeshVertexVaryingEnableBlending* mesh_vertex_blending_varyings =
            reinterpret_cast<MeshVertex::VulkanMeshVertexVaryingEnableBlending*>(
                reinterpret_cast<uintptr_t>(inefficient_staging_buffer_data) +
                vertex_varying_enable_blending_buffer_offset);

        MeshVertex::VulkanMeshVertexVarying* mesh_vertex_varyings =
            reinterpret_cast<MeshVertex::VulkanMeshVertexVarying*>(
                reinterpret_cast<uintptr_t>(inefficient_staging_buffer_data) + vertex_varying_buffer_offset);

        for (uint32_t vertex_index = 0; vertex_index < vertex_count; ++vertex_index) ///< Iterative Vertex
        {
            // Manipulating mesh_vertex_positions is essentially manipulating inefficient_staging_buffer_data.
            mesh_vertex_positions[vertex_index].position = Vector3(vertex_buffer_data[vertex_index].x,
                                                                   vertex_buffer_data[vertex_index].y,
                                                                   vertex_buffer_data[vertex_index].z);

            Vector3 normal                                     = Vector3(vertex_buffer_data[vertex_index].nx,
                                     vertex_buffer_data[vertex_index].ny,
                                     vertex_buffer_data[vertex_index].nz);
            mesh_vertex_blending_varyings[vertex_index].normal = normal;

            mesh_vertex_varyings[vertex_index].texcoord =
                Vector2(vertex_buffer_data[vertex_index].u, vertex_buffer_data[vertex_index].v);
        }

        vkUnmapMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory);

        // use the vmaAllocator to allocate asset vertex buffer
        VkBufferCreateInfo bufferInfo = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferInfo.usage              = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage                   = VMA_MEMORY_USAGE_GPU_ONLY;

        bufferInfo.size = vertex_position_buffer_size;
        vmaCreateBuffer(vulkan_api->getVmaAllocator(),
                        &bufferInfo,
                        &allocInfo,
                        &now_mesh.mesh_vertex_position_buffer,
                        &now_mesh.mesh_vertex_position_buffer_allocation,
                        NULL);
        bufferInfo.size = vertex_varying_enable_blending_buffer_size;
        vmaCreateBuffer(vulkan_api->getVmaAllocator(),
                        &bufferInfo,
                        &allocInfo,
                        &now_mesh.mesh_vertex_varying_enable_blending_buffer,
                        &now_mesh.mesh_vertex_varying_enable_blending_buffer_allocation,
                        NULL);
        bufferInfo.size = vertex_varying_buffer_size;
        vmaCreateBuffer(vulkan_api->getVmaAllocator(),
                        &bufferInfo,
                        &allocInfo,
                        &now_mesh.mesh_vertex_varying_buffer,
                        &now_mesh.mesh_vertex_varying_buffer_allocation,
                        NULL);

        // copy data from staging buffer
        vulkan_api->copyBuffer(inefficient_staging_buffer,
                               now_mesh.mesh_vertex_position_buffer,
                               vertex_position_buffer_offset,
                               0,
                               vertex_position_buffer_size);
        vulkan_api->copyBuffer(inefficient_staging_buffer,
                               now_mesh.mesh_vertex_varying_enable_blending_buffer,
                               vertex_varying_enable_blending_buffer_offset,
                               0,
                               vertex_varying_enable_blending_buffer_size);
        vulkan_api->copyBuffer(inefficient_staging_buffer,
                               now_mesh.mesh_vertex_varying_buffer,
                               vertex_varying_buffer_offset,
                               0,
                               vertex_varying_buffer_size);

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

    void RenderResource::updateTextureImageData(std::shared_ptr<VulkanAPI> vulkan_api,
                                                const TextureDataToUpdate& texture_data)
    {
        vulkan_api->createGlobalImage(texture_data.now_material->base_color_texture_image,
                                      texture_data.now_material->base_color_image_view,
                                      texture_data.now_material->base_color_image_allocation,
                                      texture_data.base_color_image_width,
                                      texture_data.base_color_image_height,
                                      texture_data.base_color_image_pixels,
                                      texture_data.base_color_image_format);
    }
} // namespace VKernel
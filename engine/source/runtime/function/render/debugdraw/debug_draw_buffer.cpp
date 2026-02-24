#include "runtime/function/render/debugdraw/debug_draw_buffer.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_resource_base.h"
#include "runtime/function/render/render_system.h"

#include "runtime/core/base/macro.h"

#include "debug_draw_buffer.h"
#include <iostream>

namespace VKernel
{
    void DebugDrawAllocator::initialize()
    {
        m_vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();

        // create DescriptorLayout and DescriptorSet
        setupDescriptorSet();
        prepareDescriptorSet();
    }

    void DebugDrawAllocator::tick()
    {
        flushPendingDelete();                                                             ///< deferred delete
        m_current_frame = (m_current_frame + 1) % k_deferred_delete_resource_frame_count; ///< update frame index
    }

    void DebugDrawAllocator::destory()
    {
        clear();
        clearLines();
    }

    void DebugDrawAllocator::clear()
    {
        // clear buffer
        clearBuffer();

        // clear cache
        m_vertex_cache.clear();
        m_indice_cache.clear();
        m_uniform_buffer_object.proj_view_matrix = Matrix4x4::IDENTITY;
        m_uniform_buffer_dynamic_object_cache.clear();
    }

    void DebugDrawAllocator::clearLines()
    {
        clearBufferLines();

        m_vertex_cache_lines.clear();
        m_uniform_buffer_dynamic_object_cache_lines.clear();
    }

    void DebugDrawAllocator::clearBuffer()
    {
        // clear buffer and memory
        if (m_indice_resource.buffer)
        {
            m_deffer_delete_queue[m_current_frame].push(m_indice_resource);
            m_indice_resource.buffer = nullptr;
            m_indice_resource.memory = nullptr;
        }
        if (m_uniform_resource.buffer)
        {
            m_deffer_delete_queue[m_current_frame].push(m_uniform_resource);
            m_uniform_resource.buffer = nullptr;
            m_uniform_resource.memory = nullptr;
        }
        if (m_uniform_dynamic_resource.buffer)
        {
            m_deffer_delete_queue[m_current_frame].push(m_uniform_dynamic_resource);
            m_uniform_dynamic_resource.buffer = nullptr;
            m_uniform_dynamic_resource.memory = nullptr;
        }
        if (m_vertex_resource.buffer)
        {
            m_deffer_delete_queue[m_current_frame].push(m_vertex_resource);
            m_vertex_resource.buffer = nullptr;
            m_vertex_resource.memory = nullptr;
        }
    }

    void DebugDrawAllocator::clearBufferLines()
    {
        // clear buffer and memory
        if (m_uniform_dynamic_resource_lines.buffer)
        {
            m_deffer_delete_queue[m_current_frame].push(m_uniform_dynamic_resource_lines);
            m_uniform_dynamic_resource_lines.buffer = nullptr;
            m_uniform_dynamic_resource_lines.memory = nullptr;
        }
        if (m_vertex_resource_lines.buffer)
        {
            m_deffer_delete_queue[m_current_frame].push(m_vertex_resource_lines);
            m_vertex_resource_lines.buffer = nullptr;
            m_vertex_resource_lines.memory = nullptr;
        }
    }

    void DebugDrawAllocator::cacheVertexs(const std::vector<DebugDrawVertex>& vertexs)
    {
        m_vertex_cache.insert(m_vertex_cache.end(), vertexs.begin(), vertexs.end());
    }

    void DebugDrawAllocator::cacheVertexsLines(const std::vector<DebugDrawVertex>& vertexs)
    {
        m_vertex_cache_lines.insert(m_vertex_cache_lines.end(), vertexs.begin(), vertexs.end());
    }

    void DebugDrawAllocator::cacheIndices(const std::vector<uint16_t>& indices)
    {
        m_indice_cache.insert(m_indice_cache.end(), indices.begin(), indices.end());
    }

    void DebugDrawAllocator::cacheUniformObject(Matrix4x4 proj_view_matrix)
    {
        m_uniform_buffer_object.proj_view_matrix = proj_view_matrix;
    }

    size_t
    DebugDrawAllocator::cacheUniformDynamicObject(const std::vector<std::tuple<Matrix4x4, Vector4, uint32_t>>& datas)
    {
        size_t offset = m_uniform_buffer_dynamic_object_cache.size(); ///< start offset
        m_uniform_buffer_dynamic_object_cache.resize(offset + datas.size());
        for (size_t i = 0; i < datas.size(); i++)
        {
            m_uniform_buffer_dynamic_object_cache[i + offset].model_matrix = std::get<0>(datas[i]);
            m_uniform_buffer_dynamic_object_cache[i + offset].color        = std::get<1>(datas[i]);
            m_uniform_buffer_dynamic_object_cache[i + offset].texture_type = std::get<2>(datas[i]);
        }
        return offset;
    }

    size_t DebugDrawAllocator::cacheUniformDynamicObjectLines(
        const std::vector<std::tuple<Matrix4x4, Vector4, uint32_t>>& datas)
    {
        size_t offset = m_uniform_buffer_dynamic_object_cache_lines.size(); ///< start offset
        m_uniform_buffer_dynamic_object_cache_lines.resize(offset + datas.size());
        for (size_t i = 0; i < datas.size(); i++)
        {
            m_uniform_buffer_dynamic_object_cache_lines[i + offset].model_matrix = std::get<0>(datas[i]);
            m_uniform_buffer_dynamic_object_cache_lines[i + offset].color        = std::get<1>(datas[i]);
            m_uniform_buffer_dynamic_object_cache_lines[i + offset].texture_type = std::get<2>(datas[i]);
        }
        return offset;
    }

    void DebugDrawAllocator::allocator()
    {
        clearBuffer();

        // vertex
        uint64_t bufferSize = static_cast<uint64_t>(m_vertex_cache.size() * sizeof(DebugDrawVertex));
        if (bufferSize > 0)
        {
            // create buffer
            m_vulkan_api->createBuffer(bufferSize,
                                       VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                       VMA_MEMORY_USAGE_GPU_ONLY,
                                       m_vertex_resource.buffer,
                                       m_vertex_resource.memory);

            // create temporary staging buffer
            Resource stagingBuffer;
            m_vulkan_api->createBuffer(bufferSize,
                                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                       stagingBuffer.buffer,
                                       stagingBuffer.memory);
            void* data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), stagingBuffer.memory, 0, bufferSize, 0, &data);
            memcpy(data, m_vertex_cache.data(), bufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), stagingBuffer.memory);

            // copy data from staging buffer
            m_vulkan_api->copyBuffer(stagingBuffer.buffer, m_vertex_resource.buffer, 0, 0, bufferSize);

            // release staging buffer
            vkDestroyBuffer(m_vulkan_api->getLogicDevice(), stagingBuffer.buffer, nullptr);
            vkFreeMemory(m_vulkan_api->getLogicDevice(), stagingBuffer.memory, nullptr);
        }

        // indice
        uint64_t indice_bufferSize = static_cast<uint64_t>(m_indice_cache.size() * sizeof(uint16_t));
        if (indice_bufferSize > 0)
        {
            // create memory and buffer
            m_vulkan_api->createBuffer(indice_bufferSize,
                                       VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                       m_indice_resource.buffer,
                                       m_indice_resource.memory);

            // Fill the indice data into the memory
            void* data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), m_indice_resource.memory, 0, indice_bufferSize, 0, &data);
            memcpy(data, m_indice_cache.data(), indice_bufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_indice_resource.memory);
        }

        // ubo
        uint64_t uniform_BufferSize = static_cast<uint64_t>(sizeof(UniformBufferObject));
        if (uniform_BufferSize > 0)
        {
            // create memory and buffer
            m_vulkan_api->createBuffer(uniform_BufferSize,
                                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                       m_uniform_resource.buffer,
                                       m_uniform_resource.memory);

            // Fill the vertex data into the memory
            void* data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), m_uniform_resource.memory, 0, uniform_BufferSize, 0, &data);
            memcpy(data, &m_uniform_buffer_object.proj_view_matrix, uniform_BufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_uniform_resource.memory);
        }

        // udbo
        uint64_t uniform_dynamic_BufferSize =
            static_cast<uint64_t>(sizeof(UniformBufferDynamicObject) * m_uniform_buffer_dynamic_object_cache.size());
        if (uniform_dynamic_BufferSize > 0)
        {
            // create memory and buffer
            m_vulkan_api->createBuffer(uniform_dynamic_BufferSize,
                                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                       m_uniform_dynamic_resource.buffer,
                                       m_uniform_dynamic_resource.memory);

            // Fill the vertex data into the memory
            void* data;
            vkMapMemory(m_vulkan_api->getLogicDevice(),
                        m_uniform_dynamic_resource.memory,
                        0,
                        uniform_dynamic_BufferSize,
                        0,
                        &data);
            memcpy(data, m_uniform_buffer_dynamic_object_cache.data(), uniform_dynamic_BufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_uniform_dynamic_resource.memory);
        }

        updateDescriptorSet();
    }

    void DebugDrawAllocator::allocatorLines()
    {
        // clear buffer and memory
        clearBufferLines();

        // vertex
        uint64_t bufferSize = static_cast<uint64_t>(m_vertex_cache_lines.size() * sizeof(DebugDrawVertex));
        if (bufferSize > 0)
        {
            // create buffer
            m_vulkan_api->createBuffer(bufferSize,
                                       VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                       VMA_MEMORY_USAGE_GPU_ONLY,
                                       m_vertex_resource_lines.buffer,
                                       m_vertex_resource_lines.memory);

            // create temporary staging buffer
            Resource stagingBuffer;
            m_vulkan_api->createBuffer(bufferSize,
                                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                       stagingBuffer.buffer,
                                       stagingBuffer.memory);
            void* data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), stagingBuffer.memory, 0, bufferSize, 0, &data);
            memcpy(data, m_vertex_cache_lines.data(), bufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), stagingBuffer.memory);

            // copy data from staging buffer
            m_vulkan_api->copyBuffer(stagingBuffer.buffer, m_vertex_resource_lines.buffer, 0, 0, bufferSize);

            // release staging buffer
            vkDestroyBuffer(m_vulkan_api->getLogicDevice(), stagingBuffer.buffer, nullptr);
            vkFreeMemory(m_vulkan_api->getLogicDevice(), stagingBuffer.memory, nullptr);
        }

        // udbo
        uint64_t uniform_dynamic_BufferSize = static_cast<uint64_t>(sizeof(UniformBufferDynamicObject) *
                                                                    m_uniform_buffer_dynamic_object_cache_lines.size());
        if (uniform_dynamic_BufferSize > 0)
        {
            // create memory and buffer
            m_vulkan_api->createBuffer(uniform_dynamic_BufferSize,
                                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                       m_uniform_dynamic_resource_lines.buffer,
                                       m_uniform_dynamic_resource_lines.memory);

            // Fill the vertex data into the memory
            void* data;
            vkMapMemory(m_vulkan_api->getLogicDevice(),
                        m_uniform_dynamic_resource_lines.memory,
                        0,
                        uniform_dynamic_BufferSize,
                        0,
                        &data);
            memcpy(data, m_uniform_buffer_dynamic_object_cache_lines.data(), uniform_dynamic_BufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_uniform_dynamic_resource_lines.memory);
        }

        updateDescriptorSetLines();
    }

    VkBuffer DebugDrawAllocator::getIndiceBuffer() const { return m_indice_resource.buffer; }

    VkDescriptorSet DebugDrawAllocator::getDescriptorSet() const
    {
        return m_descriptor.descriptor_set[m_vulkan_api->getCurrentFrameIndex()];
    }

    VkDescriptorSet DebugDrawAllocator::getDescriptorSetLines() const
    {
        return m_descriptor.descriptor_set_lines[m_vulkan_api->getCurrentFrameIndex()];
    }

    VkBuffer DebugDrawAllocator::getVertexBuffer() const { return m_vertex_resource.buffer; }

    VkBuffer DebugDrawAllocator::getVertexBufferLines() const { return m_vertex_resource_lines.buffer; }

    const size_t DebugDrawAllocator::getSizeOfUniformBufferObject() const { return sizeof(UniformBufferDynamicObject); }

    size_t DebugDrawAllocator::getVertexCacheOffset() const { return m_vertex_cache.size(); }

    void DebugDrawAllocator::setupDescriptorSet()
    {
        // create DescriptorSetLayout
        VkDescriptorSetLayoutBinding uboLayoutBinding[3];
        uboLayoutBinding[0].binding            = 0;
        uboLayoutBinding[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding[0].descriptorCount    = 1;
        uboLayoutBinding[0].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding[0].pImmutableSamplers = nullptr;

        uboLayoutBinding[1].binding            = 1;
        uboLayoutBinding[1].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        uboLayoutBinding[1].descriptorCount    = 1;
        uboLayoutBinding[1].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding[1].pImmutableSamplers = nullptr;

        uboLayoutBinding[2].binding            = 2;
        uboLayoutBinding[2].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        uboLayoutBinding[2].descriptorCount    = TextureType::TEXTURE_TYPE_COUNT - 1;
        uboLayoutBinding[2].stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
        uboLayoutBinding[2].pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo layoutInfo {};
        layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 3;
        layoutInfo.pBindings    = uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(m_vulkan_api->getLogicDevice(), &layoutInfo, nullptr, &m_descriptor.layout) !=
            VK_SUCCESS)
        {
            LOG_ERROR("create debug draw layout");
        }

        // allocate DescriptorSet
        m_descriptor.descriptor_set.resize(m_vulkan_api->getMaxFramesInFlight());
        for (size_t i = 0; i < m_vulkan_api->getMaxFramesInFlight(); i++)
        {
            VkDescriptorSetAllocateInfo allocInfo {};
            allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.pNext              = NULL;
            allocInfo.descriptorPool     = m_vulkan_api->getDescriptorPool();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts        = &m_descriptor.layout;

            if (VK_SUCCESS !=
                vkAllocateDescriptorSets(m_vulkan_api->getLogicDevice(), &allocInfo, &m_descriptor.descriptor_set[i]))
            {
                LOG_ERROR("debug draw descriptor set");
            }
        }
        m_descriptor.descriptor_set_lines.resize(m_vulkan_api->getMaxFramesInFlight());
        for (size_t i = 0; i < m_vulkan_api->getMaxFramesInFlight(); i++)
        {
            VkDescriptorSetAllocateInfo allocInfo {};
            allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.pNext              = NULL;
            allocInfo.descriptorPool     = m_vulkan_api->getDescriptorPool();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts        = &m_descriptor.layout;

            if (VK_SUCCESS != vkAllocateDescriptorSets(
                                  m_vulkan_api->getLogicDevice(), &allocInfo, &m_descriptor.descriptor_set_lines[i]))
            {
                LOG_ERROR("debug draw descriptor set");
            }
        }
    }

    void DebugDrawAllocator::prepareDescriptorSet()
    {
        // load texture data
        std::shared_ptr<TextureData> base_color_texture[TextureType::TEXTURE_TYPE_COUNT];
        base_color_texture[0] = g_runtime_global_context.m_render_system->getRenderResource()->loadTexture(
            "asset/objects/_textures/gold.tga", false);
        base_color_texture[1] = g_runtime_global_context.m_render_system->getRenderResource()->loadTexture(
            "asset/objects/_textures/floor.jpg", false);
        base_color_texture[2] = g_runtime_global_context.m_render_system->getRenderResource()->loadTexture(
            "asset/objects/_textures/wood.png", false);
        base_color_texture[3] = g_runtime_global_context.m_render_system->getRenderResource()->loadTexture(
            "asset/objects/_textures/block.png", false);
        base_color_texture[4] = g_runtime_global_context.m_render_system->getRenderResource()->loadTexture(
            "asset/objects/_textures/brickwall.jpg", false);
        base_color_texture[5] = g_runtime_global_context.m_render_system->getRenderResource()->loadTexture(
            "asset/objects/_textures/bricks2.jpg", false);

        // image and DescriptorImageInfo
        VkDescriptorImageInfo image_info[TextureType::TEXTURE_TYPE_COUNT];
        for (size_t i = 0; i < (TextureType::TEXTURE_TYPE_COUNT - 1); i++)
        {
            // create global image

            m_vulkan_api->createGlobalImage(image[i],
                                            imageView[i],
                                            m_allocation[i],
                                            base_color_texture[i]->m_width,
                                            base_color_texture[i]->m_height,
                                            base_color_texture[i]->m_pixels,
                                            base_color_texture[i]->m_format);

            // DescriptorSet bind buffer
            image_info[i].imageView   = imageView[i];
            image_info[i].sampler     = m_vulkan_api->getOrCreateDefaultSampler(Default_Sampler_Linear);
            image_info[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }

        for (size_t i = 0; i < m_vulkan_api->getMaxFramesInFlight(); i++)
        {
            VkWriteDescriptorSet descriptor_write[1];
            descriptor_write[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_write[0].dstSet           = m_descriptor.descriptor_set[i];
            descriptor_write[0].dstBinding       = 2;
            descriptor_write[0].dstArrayElement  = 0;
            descriptor_write[0].pNext            = nullptr;
            descriptor_write[0].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptor_write[0].descriptorCount  = TextureType::TEXTURE_TYPE_COUNT - 1;
            descriptor_write[0].pBufferInfo      = nullptr;
            descriptor_write[0].pImageInfo       = image_info;
            descriptor_write[0].pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(m_vulkan_api->getLogicDevice(), 1, descriptor_write, 0, nullptr);
        }
    }

    void DebugDrawAllocator::updateDescriptorSet()
    {
        // DescriptorSet bind buffer
        VkDescriptorBufferInfo buffer_info[2];
        buffer_info[0].buffer = m_uniform_resource.buffer;
        buffer_info[0].offset = 0;
        buffer_info[0].range  = sizeof(UniformBufferObject);

        buffer_info[1].buffer = m_uniform_dynamic_resource.buffer;
        buffer_info[1].offset = 0;
        buffer_info[1].range  = sizeof(UniformBufferDynamicObject);

        VkWriteDescriptorSet descriptor_write[2];
        descriptor_write[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write[0].dstSet           = m_descriptor.descriptor_set[m_vulkan_api->getCurrentFrameIndex()];
        descriptor_write[0].dstBinding       = 0;
        descriptor_write[0].dstArrayElement  = 0;
        descriptor_write[0].pNext            = nullptr;
        descriptor_write[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_write[0].descriptorCount  = 1;
        descriptor_write[0].pBufferInfo      = &buffer_info[0];
        descriptor_write[0].pImageInfo       = nullptr;
        descriptor_write[0].pTexelBufferView = nullptr;

        descriptor_write[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write[1].dstSet           = m_descriptor.descriptor_set[m_vulkan_api->getCurrentFrameIndex()];
        descriptor_write[1].dstBinding       = 1;
        descriptor_write[1].dstArrayElement  = 0;
        descriptor_write[1].pNext            = nullptr;
        descriptor_write[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptor_write[1].descriptorCount  = 1;
        descriptor_write[1].pBufferInfo      = &buffer_info[1];
        descriptor_write[1].pImageInfo       = nullptr;
        descriptor_write[1].pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(m_vulkan_api->getLogicDevice(), 2, descriptor_write, 0, nullptr);
    }

    void DebugDrawAllocator::updateDescriptorSetLines()
    {
        // DescriptorSet bind buffer
        VkDescriptorBufferInfo buffer_info[2];
        buffer_info[0].buffer = m_uniform_resource.buffer;
        buffer_info[0].offset = 0;
        buffer_info[0].range  = sizeof(UniformBufferObject);

        buffer_info[1].buffer = m_uniform_dynamic_resource_lines.buffer;
        buffer_info[1].offset = 0;
        buffer_info[1].range  = sizeof(UniformBufferDynamicObject);

        VkWriteDescriptorSet descriptor_write[2];
        descriptor_write[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write[0].dstSet           = m_descriptor.descriptor_set_lines[m_vulkan_api->getCurrentFrameIndex()];
        descriptor_write[0].dstBinding       = 0;
        descriptor_write[0].dstArrayElement  = 0;
        descriptor_write[0].pNext            = nullptr;
        descriptor_write[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_write[0].descriptorCount  = 1;
        descriptor_write[0].pBufferInfo      = &buffer_info[0];
        descriptor_write[0].pImageInfo       = nullptr;
        descriptor_write[0].pTexelBufferView = nullptr;

        descriptor_write[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write[1].dstSet           = m_descriptor.descriptor_set_lines[m_vulkan_api->getCurrentFrameIndex()];
        descriptor_write[1].dstBinding       = 1;
        descriptor_write[1].dstArrayElement  = 0;
        descriptor_write[1].pNext            = nullptr;
        descriptor_write[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptor_write[1].descriptorCount  = 1;
        descriptor_write[1].pBufferInfo      = &buffer_info[1];
        descriptor_write[1].pImageInfo       = nullptr;
        descriptor_write[1].pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(m_vulkan_api->getLogicDevice(), 2, descriptor_write, 0, nullptr);
    }

    void DebugDrawAllocator::flushPendingDelete()
    {
        // Calculate the indices of frames to be deleted
        uint32_t current_frame_to_delete = (m_current_frame + 1) % k_deferred_delete_resource_frame_count;

        // Iterate over each element of this frame
        while (!m_deffer_delete_queue[current_frame_to_delete].empty())
        {
            // The first element
            Resource resource_to_delete = m_deffer_delete_queue[current_frame_to_delete].front();
            m_deffer_delete_queue[current_frame_to_delete].pop();

            // If not empty
            if (resource_to_delete.buffer == nullptr || resource_to_delete.memory == nullptr)
            {
                continue;
            }

            // destory memory
            vkFreeMemory(m_vulkan_api->getLogicDevice(), resource_to_delete.memory, nullptr);
            vkDestroyBuffer(m_vulkan_api->getLogicDevice(), resource_to_delete.buffer, nullptr);
        }
    }
} // namespace VKernel
#include "runtime/function/render/debugdraw/debug_draw_buffer.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"

#include <iostream>
#include "debug_draw_buffer.h"

namespace VKernel
{
    void DebugDrawAllocator::initialize()
    {
        m_vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();

        // create DescriptorLayout and DescriptorSet
        setupDescriptorSet();
    }

    void DebugDrawAllocator::destory()
    {
        clear();
    }

    void DebugDrawAllocator::clear()
    {
        // clear buffer
        clearBuffer();

        // clear cache
        m_vertex_cache.clear();
        m_sphere_vertex_cache.clear();
        m_uniform_buffer_object.proj_view_matrix = Matrix4x4::IDENTITY;
        m_uniform_buffer_dynamic_object_cache.clear();
    }

    void DebugDrawAllocator::clearBuffer()
    {
        // clear buffer and memory
        if (m_vertex_resource.buffer)
        {
            m_vertex_resource.buffer = nullptr;
            m_vertex_resource.memory = nullptr;
        }
        if (m_uniform_resource.buffer)
        {
            m_uniform_resource.buffer = nullptr;
            m_uniform_resource.memory = nullptr;
        }
        if (m_uniform_dynamic_resource.buffer)
        {
            m_uniform_dynamic_resource.buffer = nullptr;
            m_uniform_dynamic_resource.memory = nullptr;
        }
        if (m_sphere_resource.buffer)
        {
            m_sphere_resource.buffer = nullptr;
            m_sphere_resource.memory = nullptr;
        }
    }

    size_t DebugDrawAllocator::cacheVertexs(const std::vector<DebugDrawVertex> &vertexs)
    {
        size_t offset = m_vertex_cache.size();
        m_vertex_cache.insert(m_vertex_cache.end(),
                              vertexs.begin(), vertexs.end());
        return offset;
    }

    void DebugDrawAllocator::cacheSphereVertexs(const std::vector<DebugDrawVertex> &vertexs)
    {
        m_sphere_vertex_cache.insert(m_sphere_vertex_cache.end(),
                                     vertexs.begin(), vertexs.end());
    }

    void DebugDrawAllocator::cacheIndices(const std::vector<uint8_t> &indices)
    {
        m_indice_cache.insert(m_indice_cache.end(),
                              indices.begin(), indices.end());
    }

    void DebugDrawAllocator::cacheUniformObject(Matrix4x4 proj_view_matrix)
    {
        m_uniform_buffer_object.proj_view_matrix = proj_view_matrix;
    }

    size_t DebugDrawAllocator::cacheUniformDynamicObject(const std::vector<Matrix4x4> &model)
    {
        size_t offset = m_uniform_buffer_dynamic_object_cache.size(); ///< start offset
        m_uniform_buffer_dynamic_object_cache.resize(offset + model.size());
        for (size_t i = 0; i < model.size(); i++)
        {
            m_uniform_buffer_dynamic_object_cache[offset + i].model_matrix = model[i];
        }

        return offset;
    }

    void DebugDrawAllocator::allocator()
    {
        clearBuffer();

        // vertex cache
        uint64_t vertex_bufferSize = static_cast<uint64_t>(m_vertex_cache.size() * sizeof(DebugDrawVertex));
        if (vertex_bufferSize > 0)
        {
            // create memory and buffer
            m_vulkan_api->createBuffer(
                vertex_bufferSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_vertex_resource.buffer,
                m_vertex_resource.memory);

            // Fill the vertex data into the memory
            void *data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), m_vertex_resource.memory, 0, vertex_bufferSize, 0, &data);
            memcpy(data, m_vertex_cache.data(), vertex_bufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_vertex_resource.memory);
        }

        // vertex sphere
        uint64_t bufferSize = static_cast<uint64_t>(m_sphere_vertex_cache.size() * sizeof(DebugDrawVertex));
        if (bufferSize > 0)
        {
            m_vulkan_api->createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VMA_MEMORY_USAGE_GPU_ONLY,
                m_sphere_resource.buffer,
                m_sphere_resource.memory);

            Resource stagingBuffer;
            m_vulkan_api->createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer.buffer,
                stagingBuffer.memory);
            void *data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), stagingBuffer.memory, 0, bufferSize, 0, &data);
            memcpy(data, m_sphere_vertex_cache.data(), bufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), stagingBuffer.memory);

            m_vulkan_api->copyBuffer(stagingBuffer.buffer, m_sphere_resource.buffer, 0, 0, bufferSize);

            vkDestroyBuffer(m_vulkan_api->getLogicDevice(), stagingBuffer.buffer, nullptr);
            vkFreeMemory(m_vulkan_api->getLogicDevice(), stagingBuffer.memory, nullptr);
        }

        // indice
        uint64_t indice_bufferSize = static_cast<uint64_t>(m_indice_cache.size() * sizeof(uint8_t));
        if (indice_bufferSize > 0)
        {
            // create memory and buffer
            m_vulkan_api->createBuffer(
                indice_bufferSize,
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_indice_resource.buffer,
                m_indice_resource.memory);

            // Fill the indice data into the memory
            void *data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), m_indice_resource.memory, 0, indice_bufferSize, 0, &data);
            memcpy(data, m_indice_cache.data(), indice_bufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_indice_resource.memory);
        }

        // ubo
        uint64_t uniform_BufferSize = static_cast<uint64_t>(sizeof(UniformBufferObject));
        if (uniform_BufferSize > 0)
        {
            // create memory and buffer
            m_vulkan_api->createBuffer(
                uniform_BufferSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_uniform_resource.buffer,
                m_uniform_resource.memory);

            // Fill the vertex data into the memory
            void *data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), m_uniform_resource.memory, 0, uniform_BufferSize, 0, &data);
            memcpy(data, &m_uniform_buffer_object.proj_view_matrix, uniform_BufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_uniform_resource.memory);
        }

        // udbo
        uint64_t uniform_dynamic_BufferSize = static_cast<uint64_t>(sizeof(UniformBufferDynamicObject) * m_uniform_buffer_dynamic_object_cache.size());
        if (uniform_dynamic_BufferSize > 0)
        {
            // create memory and buffer
            m_vulkan_api->createBuffer(
                uniform_dynamic_BufferSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_uniform_dynamic_resource.buffer,
                m_uniform_dynamic_resource.memory);

            // Fill the vertex data into the memory
            void *data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), m_uniform_dynamic_resource.memory, 0, uniform_dynamic_BufferSize, 0, &data);
            memcpy(data, m_uniform_buffer_dynamic_object_cache.data(), uniform_dynamic_BufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_uniform_dynamic_resource.memory);
        }

        updateDescriptorSet();
    }

    size_t DebugDrawAllocator::getVertexCacheOffset() const
    {
        return m_vertex_cache.size();
    }

    VkBuffer DebugDrawAllocator::getVertexBuffer() const { return m_vertex_resource.buffer; }

    VkBuffer DebugDrawAllocator::getIndiceBuffer() const { return m_indice_resource.buffer; }

    VkDescriptorSet DebugDrawAllocator::getDescriptorSet() const { return m_descriptor.descriptor_set[m_vulkan_api->getCurrentFrameIndex()]; }

    VkBuffer DebugDrawAllocator::getSphereVertexBuffer() const { return m_sphere_resource.buffer; }

    const size_t DebugDrawAllocator::getSizeOfUniformBufferObject() const
    {
        return sizeof(UniformBufferDynamicObject);
    }

    const size_t DebugDrawAllocator::getSphereVertexBufferSize() const
    {
        return DebugDrawSphere::SPHERE_BASIC_COUNT;
    }

    void DebugDrawAllocator::setupDescriptorSet()
    {
        // create DescriptorSetLayout
        VkDescriptorSetLayoutBinding uboLayoutBinding[2];
        uboLayoutBinding[0].binding = 0;
        uboLayoutBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding[0].descriptorCount = 1;
        uboLayoutBinding[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding[0].pImmutableSamplers = nullptr;

        uboLayoutBinding[1].binding = 1;
        uboLayoutBinding[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        uboLayoutBinding[1].descriptorCount = 1;
        uboLayoutBinding[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding[1].pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 2;
        layoutInfo.pBindings = uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(m_vulkan_api->getLogicDevice(), &layoutInfo, nullptr, &m_descriptor.layout) != VK_SUCCESS)
        {
            throw std::runtime_error("create debug draw layout");
        }

        // allocate DescriptorSet
        m_descriptor.descriptor_set.resize(m_vulkan_api->getMaxFramesInFlight());
        for (size_t i = 0; i < m_vulkan_api->getMaxFramesInFlight(); i++)
        {
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.pNext = NULL;
            allocInfo.descriptorPool = m_vulkan_api->getDescriptorPool();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &m_descriptor.layout;

            if (VK_SUCCESS != vkAllocateDescriptorSets(m_vulkan_api->getLogicDevice(), &allocInfo, &m_descriptor.descriptor_set[i]))
            {
                throw std::runtime_error("debug draw descriptor set");
            }
        }
    }

    void DebugDrawAllocator::updateDescriptorSet()
    {
        // DescriptorSet bind buffer
        VkDescriptorBufferInfo buffer_info[2];
        buffer_info[0].buffer = m_uniform_resource.buffer;
        buffer_info[0].offset = 0;
        buffer_info[0].range = sizeof(UniformBufferObject);

        buffer_info[1].buffer = m_uniform_dynamic_resource.buffer;
        buffer_info[1].offset = 0;
        buffer_info[1].range = sizeof(UniformBufferDynamicObject);

        VkWriteDescriptorSet descriptor_write[2];
        descriptor_write[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write[0].dstSet = m_descriptor.descriptor_set[m_vulkan_api->getCurrentFrameIndex()];
        descriptor_write[0].dstBinding = 0;
        descriptor_write[0].dstArrayElement = 0;
        descriptor_write[0].pNext = nullptr;
        descriptor_write[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_write[0].descriptorCount = 1;
        descriptor_write[0].pBufferInfo = &buffer_info[0];
        descriptor_write[0].pImageInfo = nullptr;
        descriptor_write[0].pTexelBufferView = nullptr;

        descriptor_write[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write[1].dstSet = m_descriptor.descriptor_set[m_vulkan_api->getCurrentFrameIndex()];
        descriptor_write[1].dstBinding = 1;
        descriptor_write[1].dstArrayElement = 0;
        descriptor_write[1].pNext = nullptr;
        descriptor_write[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptor_write[1].descriptorCount = 1;
        descriptor_write[1].pBufferInfo = &buffer_info[1];
        descriptor_write[1].pImageInfo = nullptr;
        descriptor_write[1].pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(m_vulkan_api->getLogicDevice(), 2, descriptor_write, 0, nullptr);
    }
}
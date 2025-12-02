#include "runtime/function/render/debugdraw/debug_draw_buffer.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"

#include <iostream>

namespace VKernel
{
    void DebugDrawAllocator::initialize()
    { 
        m_vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();

        setupDescriptorSet();
    }

    void DebugDrawAllocator::clear()
    {
        clearBuffer();
        m_vertex_cache.clear();
        m_uniform_buffer_object_cache.clear();
    }

    void DebugDrawAllocator::clearBuffer()
    {
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
    }
    
    size_t DebugDrawAllocator::cacheVertexs(const std::vector<DebugDrawVertex>& vertexs)
    {
        size_t offset = m_vertex_cache.size(); ///< Perhaps other data were stored previously.
        m_vertex_cache.resize(offset + vertexs.size());
        for (size_t i = 0; i < vertexs.size(); i++)
        {
            m_vertex_cache[i + offset] = vertexs[i];
        }
        return offset;
    }

    size_t DebugDrawAllocator::cacheUniformObject(const std::vector<std::pair<Matrix4x4,Matrix4x4> >& mvp)
    {
        size_t offset = m_uniform_buffer_object_cache.size();
        m_uniform_buffer_object_cache.resize(offset + mvp.size());
        for (size_t i = 0; i < mvp.size(); i++){
            m_uniform_buffer_object_cache[offset + i].model_matrix = mvp[i].first;
            m_uniform_buffer_object_cache[offset + i].proj_view_matrix = mvp[i].second;
        }

        return offset; ///< start offset
    }

    void DebugDrawAllocator::allocator()
    {
        clearBuffer();

        // vertex
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
            void* data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), m_vertex_resource.memory, 0, vertex_bufferSize, 0, &data);
            memcpy(data, m_vertex_cache.data(), vertex_bufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_vertex_resource.memory);
        }

        // ubo
        uint64_t uniform_BufferSize = static_cast<uint64_t>(sizeof(UniformBufferObject) * m_uniform_buffer_object_cache.size());
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
            void* data;
            vkMapMemory(m_vulkan_api->getLogicDevice(), m_uniform_resource.memory, 0, uniform_BufferSize, 0, &data);
            memcpy(data, m_uniform_buffer_object_cache.data(), uniform_BufferSize);
            vkUnmapMemory(m_vulkan_api->getLogicDevice(), m_uniform_resource.memory);
        }

        updateDescriptorSet();
    }

    size_t DebugDrawAllocator::getVertexCacheOffset() const
    {
        return m_vertex_cache.size();
    }

    VkBuffer DebugDrawAllocator::getVertexBuffer() const {return m_vertex_resource.buffer;}

    VkDescriptorSet DebugDrawAllocator::getDescriptorSet() const { return m_descriptor.descriptor_set[m_vulkan_api->getCurrentFrameIndex()]; }

    void DebugDrawAllocator::setupDescriptorSet()
    {
        // create DescriptorSetLayout
        VkDescriptorSetLayoutBinding uboLayoutBinding[1];
        uboLayoutBinding[0].binding = 0;
        uboLayoutBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding[0].descriptorCount = 1;
        uboLayoutBinding[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding[0].pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
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
        VkDescriptorBufferInfo buffer_info[1];
        buffer_info[0].buffer = m_uniform_resource.buffer;
        buffer_info[0].offset = 0;
        buffer_info[0].range = sizeof(UniformBufferObject);
        
        VkWriteDescriptorSet descriptor_write[1];
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

        vkUpdateDescriptorSets(m_vulkan_api->getLogicDevice(), 1, descriptor_write, 0, nullptr);
    }
}
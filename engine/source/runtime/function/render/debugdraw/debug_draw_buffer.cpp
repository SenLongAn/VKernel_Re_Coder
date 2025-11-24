#include "runtime/function/render/debugdraw/debug_draw_buffer.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"

#include <iostream>

namespace VKernel
{
    void DebugDrawAllocator::initialize()
    { 
        m_vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();
    }

    void DebugDrawAllocator::clear()
    {
        clearBuffer();
        m_vertex_cache.clear();
    }

    void DebugDrawAllocator::clearBuffer()
    {
        if (m_vertex_resource.buffer)
        {
            m_vertex_resource.buffer = nullptr;
            m_vertex_resource.memory = nullptr;
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
    }

    size_t DebugDrawAllocator::getVertexCacheOffset() const
    {
        return m_vertex_cache.size();
    }

    VkBuffer DebugDrawAllocator::getVertexBuffer(){return m_vertex_resource.buffer;}
}
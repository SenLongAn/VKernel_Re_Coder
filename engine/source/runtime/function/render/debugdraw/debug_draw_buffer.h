#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"
#include "runtime/function/render/debugdraw/debug_draw_primitive.h"

/**
 * buffer, memory, descriptorLayout, descriptorSet
 */
namespace VKernel
{
    class DebugDrawAllocator
    {

    public:
        DebugDrawAllocator() {}; ///< Constructor
        void initialize(); ///< init
        void clear(); ///< clear
        void clearBuffer(); ///< clear buffer

        size_t cacheVertexs(const std::vector<DebugDrawVertex>& vertexs); ///< Copy vertex data to this class
        
        void allocator(); ///< Fill the memory and buffer with data

        // get
        size_t getVertexCacheOffset() const; ///< get cache size
        VkBuffer getVertexBuffer();

    private:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface
        
        // What needs to be accomplished in the buffer class
        struct Resource
        {
            VkBuffer buffer;
            VkDeviceMemory memory;
        };

        Resource m_vertex_resource; ///< vertex: memory, buffer
        std::vector<DebugDrawVertex>m_vertex_cache; ///< Vertex data, vertex description(all primitive)
    };
}
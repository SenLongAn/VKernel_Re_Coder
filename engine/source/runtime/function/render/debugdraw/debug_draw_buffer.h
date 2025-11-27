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
        // init and destory
        DebugDrawAllocator() {}; ///< Constructor
        void initialize(); ///< init
        void clear(); ///< clear
        void clearBuffer(); ///< clear buffer

        // interface
        size_t cacheVertexs(const std::vector<DebugDrawVertex>& vertexs); ///< Copy vertex data to this class
        size_t cacheUniformDynamicObject(const std::vector<Matrix4x4>& model); ///< udbo
        
        void allocator(); ///< Fill the memory and buffer with data

        // get
        size_t getVertexCacheOffset() const; ///< get cache size
        VkBuffer getVertexBuffer() const;
        VkDescriptorSet getDescriptorSet() const;

    private:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface
        
        // What needs to be accomplished in the buffer class

        // memory and buffer
        struct Resource
        {
            VkBuffer buffer;
            VkDeviceMemory memory;
        };
        
        Resource m_vertex_resource; ///< vertex: memory, buffer
        std::vector<DebugDrawVertex>m_vertex_cache; ///< Vertex data, vertex description(all primitive)

        struct alignas(64) UniformBufferDynamicObject
        {
            Matrix4x4 model_matrix;
        };

        Resource m_uniform_dynamic_resource; ///< udbo
        std::vector<UniformBufferDynamicObject> m_uniform_buffer_dynamic_object_cache;

        // descriptor: layout and set
        struct Descriptor
        {
            VkDescriptorSetLayout layout;
            std::vector<VkDescriptorSet> descriptor_set;
        };

        Descriptor m_descriptor;
        
    private:
        // What needs to be accomplished in the buffer class
        void setupDescriptorSet();
        void updateDescriptorSet();
    };
}
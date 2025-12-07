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
        void initialize();       ///< init
        void destory();          ///< clear
        void clear();            ///< clear
        void clearBuffer();      ///< clear buffer

        // Fill the data into the cache
        size_t cacheVertexsHasIndice(const std::vector<DebugDrawVertex> &vertexs); ///< vertex data has indice
        size_t cacheVertexs(const std::vector<DebugDrawVertex> &vertexs);            ///< vertex data
        void cacheIndices(const std::vector<uint8_t> &indices);                    ///< indice data
        void cacheUniformObject(Matrix4x4 proj_view_matrix);                       ///< ubo
        size_t cacheUniformDynamicObject(const std::vector<Matrix4x4> &model);     ///< udbo

        void allocator(); ///< Fill the memory and buffer with data

        // get
        size_t getVertexHasIndiceCacheOffset() const;
        VkBuffer getVertexHasIndiceBuffer() const;
        VkBuffer getIndiceBuffer() const;
        VkDescriptorSet getDescriptorSet() const;
        VkBuffer getVertexBuffer() const;
        const size_t getSizeOfUniformBufferObject() const;
        size_t getVertexCacheOffset() const;
        
    private:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface

        // What needs to be accomplished in the buffer class

        // memory and buffer
        struct Resource
        {
            VkBuffer buffer;
            VkDeviceMemory memory;
        };

        // vertex
        Resource m_vertex_has_indice_resource; ///< triangle, quad, box
        std::vector<DebugDrawVertex> m_vertex_has_indice_cache;

        Resource m_vertex_resource; ///< sphere
        std::vector<DebugDrawVertex> m_vertex_cache;

        // indice
        Resource m_indice_resource;
        std::vector<uint8_t> m_indice_cache;

        // ubo
        struct UniformBufferObject
        {
            Matrix4x4 proj_view_matrix;
        };
        Resource m_uniform_resource;
        UniformBufferObject m_uniform_buffer_object;

        // udbo
        struct alignas(64) UniformBufferDynamicObject
        {
            Matrix4x4 model_matrix;
        };
        Resource m_uniform_dynamic_resource;
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
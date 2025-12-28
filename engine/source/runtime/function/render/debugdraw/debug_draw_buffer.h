#pragma once

#include "runtime/function/render/debugdraw/debug_draw_primitive.h"
#include "runtime/function/render/vulkan_interface/vulkan_api.h"

#include <queue>

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
        void tick();             ///< tick
        void destory();          ///< clear
        void clear();            ///< clear
        void clearBuffer();      ///< clear buffer

        // Fill the data into the cache
        void   cacheVertexs(const std::vector<DebugDrawVertex>& vertexs); ///< vertex data
        void   cacheIndices(const std::vector<uint16_t>& indices);        ///< indice data
        void   cacheUniformObject(Matrix4x4 proj_view_matrix);            ///< ubo
        size_t cacheUniformDynamicObject(const std::vector<std::tuple<Matrix4x4, Vector4, uint32_t>>& datas); ///< udbo

        void allocator(); ///< Fill the memory and buffer with data

        // get
        VkBuffer        getIndiceBuffer() const;
        VkDescriptorSet getDescriptorSet() const;
        VkBuffer        getVertexBuffer() const;
        const size_t    getSizeOfUniformBufferObject() const;
        size_t          getVertexCacheOffset() const;

    private:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface

        // memory and buffer
        struct Resource
        {
            VkBuffer       buffer = VK_NULL_HANDLE;
            VkDeviceMemory memory = VK_NULL_HANDLE;
        };

        // vertex
        Resource                     m_vertex_resource;
        std::vector<DebugDrawVertex> m_vertex_cache;

        // indice
        Resource              m_indice_resource;
        std::vector<uint16_t> m_indice_cache;

        // ubo
        struct UniformBufferObject
        {
            Matrix4x4 proj_view_matrix;
        };
        Resource            m_uniform_resource;
        UniformBufferObject m_uniform_buffer_object;

        // udbo
        struct alignas(256) UniformBufferDynamicObject
        {
            Matrix4x4 model_matrix;
            Vector4   color;
            uint32_t  texture_type;
        };
        Resource                                m_uniform_dynamic_resource;
        std::vector<UniformBufferDynamicObject> m_uniform_buffer_dynamic_object_cache;

        // descriptor: layout and set
        struct Descriptor
        {
            VkDescriptorSetLayout        layout;
            std::vector<VkDescriptorSet> descriptor_set;
        };

        Descriptor m_descriptor;

        // image
        VkImage       image[TextureType::TEXTURE_TYPE_COUNT - 1];
        VkImageView   imageView[TextureType::TEXTURE_TYPE_COUNT - 1];
        VmaAllocation m_allocation[TextureType::TEXTURE_TYPE_COUNT - 1];

        // resource deleter
        static const uint32_t k_deferred_delete_resource_frame_count = 5; ///< Frames before deletion count
        uint32_t              m_current_frame                        = 0;
        std::queue<Resource>
            m_deffer_delete_queue[k_deferred_delete_resource_frame_count]; ///< An array with 'count' elements, each
                                                                           ///< element being a queue

    private:
        // init
        void setupDescriptorSet();
        void prepareDescriptorSet();
        void updateDescriptorSet();

        void flushPendingDelete(); ///< deferred delete
    };
} // namespace VKernel
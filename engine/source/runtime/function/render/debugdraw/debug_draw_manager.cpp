#include "runtime/function/render/debugdraw/debug_draw_manager.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/core/math/math_headers.h"

#include <iostream>
namespace VKernel
{
    void DebugDrawManager::initialize()
    {
        m_vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();

        setupPipelines();
    }

    void DebugDrawManager::setupPipelines()
    {
        // create pipelines
        m_debug_draw_pipeline = new DebugDrawPipeline(DebugDrawPipelineType::_debug_draw_pipeline_type_triangle_no_depth_test);
        m_debug_draw_pipeline->initialize();
        
        // create buffer
        m_buffer_allocator = new DebugDrawAllocator();
        m_buffer_allocator->initialize();

        // add primitive
        m_debug_draw_group_for_render.addTriangle(Vector3(0.0, -0.5, 0.0),
                                                  Vector3(0.5, 0.5, 0.0),
                                                  Vector3(-0.5, 0.5, 0.0),
                                                  Vector4(1.0, 0.0, 0.0, 1.0),
                                                  Vector4(0.0, 1.0, 0.0, 1.0),
                                                  Vector4(0.0, 0.0, 1.0, 1.0),
                                                  Transform(Vector3(0.0, 0.0, 0.0),
                                                  Quaternion(Vector3(0.0, 0.0, 0.0)),
                                                  Vector3(0.5, 0.5, 0.5)));
        m_debug_draw_group_for_render.addTriangle(Vector3(0.0, -0.5, 0.0),
                                                  Vector3(0.5, 0.5, 0.0),
                                                  Vector3(-0.5, 0.5, 0.0),
                                                  Vector4(1.0, 0.0, 0.0, 1.0),
                                                  Vector4(0.0, 1.0, 0.0, 1.0),
                                                  Vector4(0.0, 0.0, 1.0, 1.0),
                                                  Transform(Vector3(0.5, 0.0, 0.0),
                                                  Quaternion(Vector3(0.0, 0.0, 0.0)),
                                                  Vector3(0.5, 0.5, 0.5)));
    }

    void DebugDrawManager::updateAfterRecreateSwapchain()
    {
        m_debug_draw_pipeline->recreateAfterSwapchain();
    }

    void DebugDrawManager::draw(uint32_t current_swapchain_image_index)
    {
        // viewport and scissor
        SwapChainDesc swap_chain_desc = m_vulkan_api->getSwapchainInfo();
        vkCmdSetViewport(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, &swap_chain_desc.viewport);
        vkCmdSetScissor(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, &swap_chain_desc.scissor);

        // draw obj
        drawDebugObject(current_swapchain_image_index);
    }

    void DebugDrawManager::drawDebugObject(uint32_t current_swapchain_image_index)
    {
        prepareDrawBuffer(); ///< prepare buffer
        drawPointLineTriangleBox(current_swapchain_image_index);
    }

    void DebugDrawManager::prepareDrawBuffer()
    {
        m_buffer_allocator->clear();

        // Data is loaded from the group to the buffer.
        // vbo
        std::vector<DebugDrawVertex> vertexs;
        m_debug_draw_group_for_render.writeTriangleData(vertexs); ///< Write the "group" data to the "vertex"
        m_no_depth_test_triangle_start_offset = m_buffer_allocator->cacheVertexs(vertexs); ///< Load vertex data
        m_no_depth_test_triangle_end_offset = m_buffer_allocator->getVertexCacheOffset(); ///< get size

        // udbo
        std::vector<Matrix4x4> dynamicObject = { Matrix4x4::IDENTITY };
        m_debug_draw_group_for_render.writeUniformDynamicDataToCache(dynamicObject);

        // temp
        // static float val;
        // val+=1;
        // dynamicObject[0] = Transform(Vector3(0.0, 0.0, 0.0), Quaternion(Vector3(0.0, 0.0, val)), Vector3(0.5, 0.5, 0.5)).getMatrix();

        m_buffer_allocator->cacheUniformDynamicObject(dynamicObject);

        // Load into the buffer
        m_buffer_allocator->allocator();
    }

    void DebugDrawManager::drawPointLineTriangleBox(uint32_t current_swapchain_image_index)
    {
        // bind vertex buffer
        VkBuffer vertex_buffers[] = { m_buffer_allocator->getVertexBuffer() };
        if (vertex_buffers[0] == nullptr)
        {
            return;
        }
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, vertex_buffers, offsets);

        // Begin RenderPass
        VkClearValue clear_values[1];
        clear_values[0].color = { 0.0f,0.0f,0.0f,1.0f };

        VkRenderPassBeginInfo renderpass_begin_info{};
        renderpass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpass_begin_info.renderArea.offset = { 0, 0 };
        renderpass_begin_info.renderArea.extent = m_vulkan_api->getSwapchainInfo().extent;
        renderpass_begin_info.clearValueCount = (sizeof(clear_values) / sizeof(clear_values[0]));
        renderpass_begin_info.pClearValues = clear_values;

        if(m_no_depth_test_triangle_start_offset == m_no_depth_test_triangle_end_offset) return;

        renderpass_begin_info.renderPass = m_debug_draw_pipeline->getFramebuffer().render_pass;
        renderpass_begin_info.framebuffer = m_debug_draw_pipeline->getFramebuffer().framebuffers[current_swapchain_image_index];
        vkCmdBeginRenderPass(m_vulkan_api->getCurrentCommandBuffer(), &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        // Bind Pipeline
        vkCmdBindPipeline(m_vulkan_api->getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_debug_draw_pipeline->getPipeline().pipeline);

        // bind DescriptorSet
        // uint32_t dynamicOffset = 0;
        // VkDescriptorSet descriptorSet = m_buffer_allocator->getDescriptorSet();
        // vkCmdBindDescriptorSets(m_vulkan_api->getCurrentCommandBuffer(),
        //     VK_PIPELINE_BIND_POINT_GRAPHICS,
        //     m_debug_draw_pipeline->getPipeline().layout,
        //     0,
        //     1,
        //     &descriptorSet,
        //     1,
        //     &dynamicOffset);

        // drawcall
        // vkCmdDraw(m_vulkan_api->getCurrentCommandBuffer(), m_no_depth_test_triangle_end_offset - m_no_depth_test_triangle_start_offset, 1, m_no_depth_test_triangle_start_offset, 0);

        // temp
        VkDescriptorSet descriptorSet = m_buffer_allocator->getDescriptorSet();
        uint32_t dynamicOffset0 = 0;
        vkCmdBindDescriptorSets(m_vulkan_api->getCurrentCommandBuffer(),
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_debug_draw_pipeline->getPipeline().layout,
            0,
            1,
            &descriptorSet,
            1,
            &dynamicOffset0);
        vkCmdDraw(m_vulkan_api->getCurrentCommandBuffer(), 3, 1, m_no_depth_test_triangle_start_offset, 0);

        uint32_t dynamicOffset1 = 64;
        vkCmdBindDescriptorSets(m_vulkan_api->getCurrentCommandBuffer(),
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_debug_draw_pipeline->getPipeline().layout,
            0,
            1,
            &descriptorSet,
            1,
            &dynamicOffset1);
        vkCmdDraw(m_vulkan_api->getCurrentCommandBuffer(), 3, 1, m_no_depth_test_triangle_start_offset + 3, 0);

        vkCmdEndRenderPass(m_vulkan_api->getCurrentCommandBuffer());
    }
}
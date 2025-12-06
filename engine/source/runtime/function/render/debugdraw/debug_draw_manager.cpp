#include "runtime/function/render/debugdraw/debug_draw_manager.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/core/math/math_headers.h"

#include <iostream>
#include "debug_draw_manager.h"
namespace VKernel
{
    void DebugDrawManager::initialize()
    {
        // load vulkan api
        m_vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();

        // init
        setupPipelines();
    }

    void DebugDrawManager::destory()
    {
        m_buffer_allocator->destory();
        delete m_buffer_allocator;
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
        const uint8_t triangle_indices[3] = {0, 1, 2};
        m_debug_draw_group_for_render.addTriangle(Vector3(0.0, -0.5, 0.0),
                                                  Vector3(0.5, 0.5, 0.0),
                                                  Vector3(-0.5, 0.5, 0.0),
                                                  Vector4(1.0, 0.0, 0.0, 1.0),
                                                  Vector4(0.0, 1.0, 0.0, 1.0),
                                                  Vector4(0.0, 0.0, 1.0, 1.0),
                                                  Transform(Vector3(-0.5, 0.0, 0.0),
                                                            Quaternion(Vector3(0.0, 0.0, 0.0)),
                                                            Vector3(0.5, 0.5, 0.5)),
                                                  triangle_indices);
        m_debug_draw_group_for_render.addTriangle(Vector3(0.0, -0.5, 0.0),
                                                  Vector3(0.5, 0.5, 0.0),
                                                  Vector3(-0.5, 0.5, 0.0),
                                                  Vector4(1.0, 0.0, 0.0, 1.0),
                                                  Vector4(0.0, 1.0, 0.0, 1.0),
                                                  Vector4(0.0, 0.0, 1.0, 1.0),
                                                  Transform(Vector3(1.0, 0.0, 0.0),
                                                            Quaternion(Vector3(0.0, 0.0, 0.0)),
                                                            Vector3(0.5, 0.5, 0.5)),
                                                  triangle_indices);
        const uint8_t quad_indices[6] = {0, 1, 2, 2, 3, 0};
        m_debug_draw_group_for_render.addQuad(Vector3(-0.5, -0.5, 0.0),
                                              Vector3(0.5, -0.5, 0.0),
                                              Vector3(0.5, 0.5, 0.0),
                                              Vector3(-0.5, 0.5, 0.0),

                                              Vector4(1.0, 0.0, 0.0, 1.0),
                                              Vector4(0.0, 1.0, 0.0, 1.0),
                                              Vector4(0.0, 0.0, 1.0, 1.0),
                                              Vector4(1.0, 1.0, 1.0, 1.0),
                                              Transform(Vector3(0.5, 0.0, 0.0),
                                                        Quaternion(Vector3(0.0, 0.0, 0.0)),
                                                        Vector3(0.5, 0.5, 0.5)),
                                              quad_indices);
        m_debug_draw_group_for_render.addQuad(Vector3(-0.5, -0.5, 0.0),
                                              Vector3(0.5, -0.5, 0.0),
                                              Vector3(0.5, 0.5, 0.0),
                                              Vector3(-0.5, 0.5, 0.0),

                                              Vector4(1.0, 0.0, 0.0, 1.0),
                                              Vector4(0.0, 1.0, 0.0, 1.0),
                                              Vector4(0.0, 0.0, 1.0, 1.0),
                                              Vector4(0.0, 0.0, 0.0, 1.0),
                                              Transform(Vector3(0.5, 0.0, 1.0),
                                                        Quaternion(Vector3(0.0, 0.0, 0.0)),
                                                        Vector3(0.5, 0.5, 0.5)),
                                              quad_indices);
        const uint8_t box_indices[36] = {0, 1, 2, 2, 3, 0,
                                         4, 5, 6, 6, 7, 4,
                                         0, 3, 7, 7, 4, 0,
                                         1, 2, 6, 6, 5, 1,
                                         0, 1, 5, 5, 4, 0,
                                         3, 2, 6, 6, 7, 3};
        m_debug_draw_group_for_render.addBox(Vector3(0.0, 0.0, 0.0),
                                             Vector3(1.0, 1.0, 1.0),

                                             Vector4(1.0, 0.0, 0.0, 1.0),
                                             Vector4(0.0, 1.0, 0.0, 1.0),
                                             Vector4(0.0, 0.0, 1.0, 1.0),
                                             Vector4(1.0, 1.0, 1.0, 1.0),
                                             Vector4(1.0, 0.0, 0.0, 1.0),
                                             Vector4(0.0, 1.0, 0.0, 1.0),
                                             Vector4(0.0, 0.0, 1.0, 1.0),
                                             Vector4(1.0, 1.0, 1.0, 1.0),

                                             Transform(Vector3(0.0, 0.0, 3.0),
                                                       Quaternion(Vector3(0.0, 0.0, 0.0)),
                                                       Vector3(0.5, 0.5, 0.5)),
                                             box_indices);
        m_debug_draw_group_for_render.addBox(Vector3(0.0, 0.0, 0.0),
                                             Vector3(1.0, 1.0, 1.0),

                                             Vector4(1.0, 0.0, 0.0, 1.0),
                                             Vector4(0.0, 1.0, 0.0, 1.0),
                                             Vector4(0.0, 0.0, 1.0, 1.0),
                                             Vector4(1.0, 1.0, 1.0, 1.0),
                                             Vector4(1.0, 0.0, 0.0, 1.0),
                                             Vector4(0.0, 1.0, 0.0, 1.0),
                                             Vector4(0.0, 0.0, 1.0, 1.0),
                                             Vector4(1.0, 1.0, 1.0, 1.0),

                                             Transform(Vector3(3.0, 0.0, 3.0),
                                                       Quaternion(Vector3(0.0, 0.0, 0.0)),
                                                       Vector3(0.5, 0.5, 0.5)),
                                             box_indices);
    }

    void DebugDrawManager::updateAfterRecreateSwapchain()
    {
        m_debug_draw_pipeline->recreateAfterSwapchain();
    }

    void DebugDrawManager::preparePassData(std::shared_ptr<RenderResourceBase> render_resource)
    {
        // Load data from the resource
        const RenderResource *resource = static_cast<const RenderResource *>(render_resource.get());
        m_proj_view_matrix = resource->m_mesh_perframe_storage_buffer_object.proj_view_matrix;
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
        prepareDrawBuffer();                                     ///< prepare buffer
        drawPointLineTriangleBox(current_swapchain_image_index); ///< render
    }

    void DebugDrawManager::prepareDrawBuffer()
    {
        // clear buffer
        m_buffer_allocator->clear();

        // Obtain the data and write it to the Cache
        // vbo
        std::vector<DebugDrawVertex> vertexs;
        m_debug_draw_group_for_render.writeTriangleData(vertexs);                          ///< Write the "group" data to the "vertex"
        m_no_depth_test_triangle_start_offset = m_buffer_allocator->cacheVertexs(vertexs); ///< Load vertex data
        m_no_depth_test_triangle_end_offset = m_buffer_allocator->getVertexCacheOffset();  ///< get size
        m_debug_draw_group_for_render.writeQuadData(vertexs);
        m_no_depth_test_quad_start_offset = m_buffer_allocator->cacheVertexs(vertexs);
        m_no_depth_test_quad_end_offset = m_buffer_allocator->getVertexCacheOffset();
        m_debug_draw_group_for_render.writeBoxData(vertexs);
        m_no_depth_test_box_start_offset = m_buffer_allocator->cacheVertexs(vertexs);
        m_no_depth_test_box_end_offset = m_buffer_allocator->getVertexCacheOffset();
        // ibo
        std::vector<uint8_t> indices;
        m_debug_draw_group_for_render.writeTriangleIndiceData(indices);
        m_buffer_allocator->cacheIndices(indices);
        m_debug_draw_group_for_render.writeQuadIndiceData(indices);
        m_buffer_allocator->cacheIndices(indices);
        m_debug_draw_group_for_render.writeBoxIndiceData(indices);
        m_buffer_allocator->cacheIndices(indices);
        // ubo
        m_buffer_allocator->cacheUniformObject(m_proj_view_matrix); ///< vp
        // udbo
        std::vector<Matrix4x4> object = {};
        m_debug_draw_group_for_render.writeUniformDynamicDataToCache(object); ///< m
        m_buffer_allocator->cacheUniformDynamicObject(object);

        // Create a buffer and bind it to the description
        m_buffer_allocator->allocator();
    }

    void DebugDrawManager::drawPointLineTriangleBox(uint32_t current_swapchain_image_index)
    {
        // bind vertex buffer
        VkBuffer vertex_buffers[] = {m_buffer_allocator->getVertexBuffer()};
        if (vertex_buffers[0] == nullptr)
        {
            return;
        }
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, vertex_buffers, offsets);

        // bind indice buffer
        VkBuffer indice_buffers = m_buffer_allocator->getIndiceBuffer();
        VkDeviceSize offset = 0;
        vkCmdBindIndexBuffer(m_vulkan_api->getCurrentCommandBuffer(), indice_buffers, offset, VK_INDEX_TYPE_UINT8_EXT);

        // primitive vertex offsets
        std::vector<size_t> vc_start_offsets{
            m_no_depth_test_triangle_start_offset,
            m_no_depth_test_quad_start_offset,
            m_no_depth_test_box_start_offset};
        std::vector<size_t> vc_end_offsets{
            m_no_depth_test_triangle_end_offset,
            m_no_depth_test_quad_end_offset,
            m_no_depth_test_box_end_offset};

        // Begin RenderPass
        VkClearValue clear_values[2];
        clear_values[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clear_values[1].depthStencil = {1.0f, 0};

        VkRenderPassBeginInfo renderpass_begin_info{};
        renderpass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpass_begin_info.renderArea.offset = {0, 0};
        renderpass_begin_info.renderArea.extent = m_vulkan_api->getSwapchainInfo().extent;
        renderpass_begin_info.clearValueCount = (sizeof(clear_values) / sizeof(clear_values[0]));
        renderpass_begin_info.pClearValues = clear_values;

        renderpass_begin_info.renderPass = m_debug_draw_pipeline->getFramebuffer().render_pass;
        renderpass_begin_info.framebuffer = m_debug_draw_pipeline->getFramebuffer().framebuffers[current_swapchain_image_index];
        vkCmdBeginRenderPass(m_vulkan_api->getCurrentCommandBuffer(), &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        // Bind Pipeline
        vkCmdBindPipeline(m_vulkan_api->getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_debug_draw_pipeline->getPipeline().pipeline);

        // drawcall
        uint32_t dynamicOffset = 0;
        size_t k = 0;
        int n = sizeof(k_primitive_vertex_counts) / sizeof(k_primitive_vertex_counts[0]);
        for (size_t i = 0; i < n; i++)
        {
            // If such primitives do not exist, skip this step.
            if (vc_end_offsets[i] - vc_start_offsets[i] == 0)
            {
                continue;
            }

            for (size_t j = vc_start_offsets[i]; j < vc_end_offsets[i]; j += k_primitive_vertex_counts[i], k += k_primitive_indice_counts[i])
            {
                // bind DescriptorSet
                VkDescriptorSet descriptorSet = m_buffer_allocator->getDescriptorSet();
                vkCmdBindDescriptorSets(m_vulkan_api->getCurrentCommandBuffer(),
                                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        m_debug_draw_pipeline->getPipeline().layout,
                                        0,
                                        1,
                                        &descriptorSet,
                                        1,
                                        &dynamicOffset);
                dynamicOffset += 64;

                // drawcall
                vkCmdDrawIndexed(m_vulkan_api->getCurrentCommandBuffer(), k_primitive_indice_counts[i], 1, k, j, 0);
            }
        }

        // end renderpass
        vkCmdEndRenderPass(m_vulkan_api->getCurrentCommandBuffer());
    }
}
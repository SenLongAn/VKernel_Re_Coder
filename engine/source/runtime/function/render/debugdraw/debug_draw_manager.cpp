#include "runtime/function/render/debugdraw/debug_draw_manager.h"

#include "runtime/core/base/macro.h"
#include "runtime/core/math/math_headers.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"

#include <iostream>

namespace VKernel
{
    void DebugDrawManager::initialize(const RenderPassInitInfo* init_info)
    {
        // load vulkan api
        m_vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();

        // write data
        const DebugPassInitInfo* _init_info = static_cast<const DebugPassInitInfo*>(init_info);
        if (init_info == nullptr)
        {
            LOG_ERROR("fxaa _init_info is nullptr");
            return;
        }

        // init
        setupPipelines(_init_info);

        // create buffer
        m_buffer_allocator = new DebugDrawAllocator();
        m_buffer_allocator->initialize();

        // add primitive
        m_debug_draw_group_for_render.addTriangle(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(-4.0, -0.5, 0.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_point,
            true);
        m_debug_draw_group_for_render.addQuad(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(-2.0, -0.5, 0.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_point,
            true);
        m_debug_draw_group_for_render.addBox(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(0.0, -0.5, 0.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_point,
            true);
        m_debug_draw_group_for_render.addSphere(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(2.0, -0.5, 0.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_point,
            true);
        m_debug_draw_group_for_render.addCylinder(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(4.0, -0.5, 0.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_point,
            true);
        m_debug_draw_group_for_render.addCapsule(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(6.0, -0.5, 0.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_point,
            true);

        m_debug_draw_group_for_render.addTriangle(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(-4.0, -0.5, 2.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_line,
            true);
        m_debug_draw_group_for_render.addQuad(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(-2.0, -0.5, 2.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_line,
            true);
        m_debug_draw_group_for_render.addBox(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(0.0, -0.5, 2.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_line,
            true);
        m_debug_draw_group_for_render.addSphere(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(2.0, -0.5, 2.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_line,
            true);
        m_debug_draw_group_for_render.addCylinder(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(4.0, -0.5, 2.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_line,
            true);
        m_debug_draw_group_for_render.addCapsule(
            Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            Transform(Vector3(6.0, -0.5, 2.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_line,
            true);

        m_debug_draw_group_for_render.addTriangle(
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            Transform(Vector3(-4.0, -0.5, 4.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_triangle,
            true,
            TextureType::GOLD);
        m_debug_draw_group_for_render.addQuad(
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            Transform(Vector3(-2.0, -0.5, 4.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_triangle,
            true,
            TextureType::FLOOR);
        m_debug_draw_group_for_render.addBox(
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            Transform(Vector3(0.0, -0.5, 4.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_triangle,
            true,
            TextureType::WOOD);
        m_debug_draw_group_for_render.addSphere(
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            Transform(Vector3(2.0, -0.5, 4.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_triangle,
            true,
            TextureType::BLOCK);
        m_debug_draw_group_for_render.addCylinder(
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            Transform(Vector3(4.0, -0.5, 4.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_triangle,
            true,
            TextureType::BRICKWALL);
        m_debug_draw_group_for_render.addCapsule(
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            Transform(Vector3(6.0, -0.5, 4.0), Quaternion(Vector3(0.0, 0.0, 0.0)), Vector3(0.5, 0.5, 0.5)),
            PrimitiveType::_Primitive_triangle,
            true,
            TextureType::CONTAINER);
    }

    void DebugDrawManager::tick(float delta_time)
    {
        // std::lock_guard<std::mutex> guard(m_mutex);
        m_buffer_allocator->tick();
    }

    void DebugDrawManager::destory()
    {
        // pipline
        for (uint8_t i = 0; i < DebugDrawPipelineType::_debug_draw_pipeline_type_count; i++)
        {
            m_debug_draw_pipeline[i]->destory();
            delete m_debug_draw_pipeline[i];
        }

        // buffer
        m_buffer_allocator->destory();
        delete m_buffer_allocator;
    }

    void DebugDrawManager::setupPipelines(const DebugPassInitInfo* init_info)
    {
        // create pipelines
        for (uint8_t i = 0; i < DebugDrawPipelineType::_debug_draw_pipeline_type_count; i++)
        {
            m_debug_draw_pipeline[i] = new DebugDrawPipeline((DebugDrawPipelineType)i);
            if (i == -1)
            {
                m_debug_draw_pipeline[i]->initialize(VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                     VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
                                                     VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
                                                     init_info->render_pass);
            }
            else
            {
                m_debug_draw_pipeline[i]->initialize(VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD,
                                                     VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                                     VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                                     init_info->render_pass);
            }
        }
    }

    void DebugDrawManager::preparePassData(std::shared_ptr<RenderResourceBase> render_resource)
    {
        // Load data from the resource
        const RenderResource* resource = static_cast<const RenderResource*>(render_resource.get());
        m_proj_view_matrix             = resource->m_mesh_perframe_storage_buffer_object.proj_view_matrix;
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
        prepareDrawBuffer();                            ///< prepare buffer
        drawSolidObject(current_swapchain_image_index); ///< render
    }

    void DebugDrawManager::prepareDrawBuffer()
    {
        // clear buffer
        m_buffer_allocator->clear();
        m_mesh_count.clear();

        // write data to the cache
        // vbo
        std::vector<DebugDrawVertex> vertexs;
        m_debug_draw_group_for_render.writeVertexData(vertexs); ///< Write the "group" data to the "vertex"
        m_buffer_allocator->cacheVertexs(vertexs);              ///< Write the "vertex" data to the "cache"

        // ibo
        std::vector<uint16_t> indices;
        m_debug_draw_group_for_render.writeIndiceData(indices);
        m_buffer_allocator->cacheIndices(indices);

        // ubo
        m_buffer_allocator->cacheUniformObject(m_proj_view_matrix); ///< vp

        // udbo
        std::vector<std::tuple<Matrix4x4, Vector4, uint32_t>> dynamicObject = {};
        m_mesh_count.push_back(m_debug_draw_group_for_render.writeUniformDynamicDataToCache(
            dynamicObject, PrimitiveType::_Primitive_point, true));
        m_buffer_allocator->cacheUniformDynamicObject(dynamicObject);
        dynamicObject.clear();
        m_mesh_count.push_back(m_debug_draw_group_for_render.writeUniformDynamicDataToCache(
            dynamicObject, PrimitiveType::_Primitive_line, true));
        m_buffer_allocator->cacheUniformDynamicObject(dynamicObject);
        dynamicObject.clear();
        m_mesh_count.push_back(m_debug_draw_group_for_render.writeUniformDynamicDataToCache(
            dynamicObject, PrimitiveType::_Primitive_triangle, true));
        m_buffer_allocator->cacheUniformDynamicObject(dynamicObject);
        dynamicObject.clear();
        m_mesh_count.push_back(m_debug_draw_group_for_render.writeUniformDynamicDataToCache(
            dynamicObject, PrimitiveType::_Primitive_point, false));
        m_buffer_allocator->cacheUniformDynamicObject(dynamicObject);
        dynamicObject.clear();
        m_mesh_count.push_back(m_debug_draw_group_for_render.writeUniformDynamicDataToCache(
            dynamicObject, PrimitiveType::_Primitive_line, false));
        m_buffer_allocator->cacheUniformDynamicObject(dynamicObject);
        dynamicObject.clear();
        m_mesh_count.push_back(m_debug_draw_group_for_render.writeUniformDynamicDataToCache(
            dynamicObject, PrimitiveType::_Primitive_triangle, false));
        m_buffer_allocator->cacheUniformDynamicObject(dynamicObject);
        dynamicObject.clear();

        // Create a buffer and bind it to the description
        m_buffer_allocator->allocator();
    }

    void DebugDrawManager::drawSolidObject(uint32_t current_swapchain_image_index)
    {
        // bind vertex buffer
        VkBuffer vertex_has_indice_buffers[] = {m_buffer_allocator->getVertexBuffer()};
        if (vertex_has_indice_buffers[0] == nullptr)
        {
            return;
        }
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, vertex_has_indice_buffers, offsets);

        // bind indice buffer
        VkBuffer     indice_buffers = m_buffer_allocator->getIndiceBuffer();
        VkDeviceSize offset         = 0;
        vkCmdBindIndexBuffer(m_vulkan_api->getCurrentCommandBuffer(), indice_buffers, offset, VK_INDEX_TYPE_UINT16);

        // piplines
        std::vector<DebugDrawPipeline*> vc_pipelines {
            m_debug_draw_pipeline[DebugDrawPipelineType::_debug_draw_pipeline_type_point],
            m_debug_draw_pipeline[DebugDrawPipelineType::_debug_draw_pipeline_type_line],
            m_debug_draw_pipeline[DebugDrawPipelineType::_debug_draw_pipeline_type_triangle],
            m_debug_draw_pipeline[DebugDrawPipelineType::_debug_draw_pipeline_type_point_no_depth_test],
            m_debug_draw_pipeline[DebugDrawPipelineType::_debug_draw_pipeline_type_line_no_depth_test],
            m_debug_draw_pipeline[DebugDrawPipelineType::_debug_draw_pipeline_type_triangle_no_depth_test]};

        // drawcall vertex has indice
        size_t uniform_dynamic_size = m_buffer_allocator->getSizeOfUniformBufferObject();
        dynamicOffset               = 0;
        size_t vertex_start_offset = 0, vertex_index = 0;
        size_t indice_start_offset = 0, indice_index = 0;

        for (size_t i = 0; i < vc_pipelines.size(); i++) ///< Iterative pipeline
        {

            // Bind Pipeline
            vkCmdBindPipeline(m_vulkan_api->getCurrentCommandBuffer(),
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              vc_pipelines[i]->getPipeline().pipeline);

            for (size_t j = 0; j < m_mesh_count[i].size(); j++) ///< Iterative mesh type
            {
                for (size_t k = 0; k < m_mesh_count[i][j]; k++) ///< Iterative mesh
                {
                    // bind DescriptorSet
                    VkDescriptorSet descriptorSet = m_buffer_allocator->getDescriptorSet();
                    vkCmdBindDescriptorSets(m_vulkan_api->getCurrentCommandBuffer(),
                                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                                            vc_pipelines[i]->getPipeline().layout,
                                            0,
                                            1,
                                            &descriptorSet,
                                            1,
                                            &dynamicOffset);
                    dynamicOffset += uniform_dynamic_size;

                    // drawcall
                    if (i == 2 || i == 5)
                    {
                        vkCmdDrawIndexed(m_vulkan_api->getCurrentCommandBuffer(),
                                         PRIMITIVE_INDICE_COUNT[indice_index],
                                         1,
                                         indice_start_offset,
                                         vertex_start_offset,
                                         0);
                    }
                    else
                    {
                        vkCmdDraw(m_vulkan_api->getCurrentCommandBuffer(),
                                  PRIMITIVE_VERTEX_COUNT[vertex_index],
                                  1,
                                  vertex_start_offset,
                                  0);
                    }
                }

                // vertex offset
                if (vertex_index >= (sizeof(PRIMITIVE_VERTEX_COUNT) / sizeof(PRIMITIVE_VERTEX_COUNT[0])))
                {
                    vertex_start_offset = 0;
                    vertex_index        = 0;
                }
                else
                {
                    vertex_start_offset += PRIMITIVE_VERTEX_COUNT[vertex_index];
                    vertex_index++;
                }

                // indice offset
                if (i == 2 || i == 5)
                {
                    if (indice_index >= (sizeof(PRIMITIVE_INDICE_COUNT) / sizeof(PRIMITIVE_INDICE_COUNT[0])))
                    {
                        indice_start_offset = 0;
                        indice_index        = 0;
                    }
                    else
                    {
                        indice_start_offset += PRIMITIVE_INDICE_COUNT[indice_index];
                        indice_index++;
                    }
                }
            }
        }
    }
} // namespace VKernel
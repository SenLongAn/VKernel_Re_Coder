#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"
#include "runtime/function/render/debugdraw/debug_draw_buffer.h"
#include "runtime/function/render/debugdraw/debug_draw_group.h"
#include "runtime/function/render/debugdraw/debug_draw_pipeline.h"

#include "runtime/function/render/render_resource.h"

#include <array>

/**
 * Basic primitive management core
 */
namespace VKernel
{
    class DebugDrawManager
    {

    public:
        // base func
        DebugDrawManager() {}; ///< Constructor
        ~DebugDrawManager() {} ///< Destructor
        void initialize();     ///< init
        void destory();        ///< clear

        // interface
        void setupPipelines(); ///< pipline and Allocator init

        void updateAfterRecreateSwapchain(); ///< destory and recreate framebuffer

        void preparePassData(std::shared_ptr<RenderResourceBase> render_resource); ///< Obtain VP data from the resource.

        // draw
        void draw(uint32_t current_swapchain_image_index); ///< Rendering interface

    private:
        // draw
        void drawDebugObject(uint32_t current_swapchain_image_index); ///< Start rendering
        void prepareDrawBuffer();                                     ///< Establish a buffer zone for the data
        void drawSolidObject(uint32_t current_swapchain_image_index); ///< draw solid object, Use indexing method

    private:
        // object reference
        std::shared_ptr<VulkanAPI> m_vulkan_api = nullptr; ///< Vulkan interface

        DebugDrawAllocator *m_buffer_allocator = nullptr;                                                      ///< buffer class
        DebugDrawGroup m_debug_draw_group_for_render;                                                          ///< primitive group
        DebugDrawPipeline *m_debug_draw_pipeline[DebugDrawPipelineType::_debug_draw_pipeline_type_count] = {}; ///< pipline

        // VP matrix
        Matrix4x4 m_proj_view_matrix;

        // mesh count offset
        std::vector<std::vector<size_t>> m_mesh_count;

        // offset
        static constexpr uint16_t PRIMITIVE_VERTEX_COUNT[] = {
            3, 4, 8, 4225, 44, 506,    // point
            6, 8, 24, 1720, 200, 1640, // line
            3, 4, 24, 4225, 44, 506     // triangle
        };
        static constexpr uint16_t PRIMITIVE_INDICE_COUNT[] = {3, 6, 36, 24576, 240, 2640};
        uint32_t dynamicOffset = 0;
    };
}
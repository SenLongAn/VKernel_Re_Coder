#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"
#include "runtime/function/render/debugdraw/debug_draw_buffer.h"
#include "runtime/function/render/debugdraw/debug_draw_group.h"
#include "runtime/function/render/debugdraw/debug_draw_pipeline.h"

#include <array>

/**
 * Basic primitive management core
 */
namespace VKernel
{
    class DebugDrawManager
    {

    public:
        DebugDrawManager() {}; ///< Constructor
        ~DebugDrawManager() { destory(); } ///< Destructor
        void initialize(); ///< init
        void destory(){}; ///< clear
        
        void setupPipelines(); ///< pipline and Allocator init

        // draw
        void draw(uint32_t current_swapchain_image_index); ///< Rendering interface
        
    private:
        // draw
        void drawDebugObject(uint32_t current_swapchain_image_index); ///< Start rendering
        void prepareDrawBuffer(); ///< Establish a buffer zone for the data
        void drawPointLineTriangleBox(uint32_t current_swapchain_image_index); ///< draw point, line ,triangle , triangle_without_depth_test
        
    private:
        std::shared_ptr<VulkanAPI> m_vulkan_api = nullptr; ///< Vulkan interface
        
        DebugDrawAllocator* m_buffer_allocator = nullptr; ///< buffer class
        DebugDrawGroup m_debug_draw_group_for_render; ///< primitive group
        DebugDrawPipeline* m_debug_draw_pipeline; ///< pipline
        
        // The starting and ending offsets in the cache of the buffer class.
        size_t m_no_depth_test_triangle_start_offset;
        size_t m_no_depth_test_triangle_end_offset;
        
    };
}
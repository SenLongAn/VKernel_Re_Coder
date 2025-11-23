#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"

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
        void draw(uint32_t current_swapchain_image_index); ///< Debug Draw core

    private:
        std::shared_ptr<VulkanAPI> m_vulkan_api = nullptr; ///< Vulkan interface
    };
}
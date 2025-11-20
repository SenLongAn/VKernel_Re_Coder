#pragma once

#include <memory>

/**
 * render core
 */
namespace VKernel
{
    class VulkanAPI;
    class WindowSystem;

    class RenderSystem
    {

    public:
        RenderSystem() = default; ///< Constructor
        ~RenderSystem(); ///< Destructor

        void initialize(std::shared_ptr<WindowSystem> window_system); ///< init
        void clear(); ///< clear

    private:
        // direct management
        std::shared_ptr<VulkanAPI> m_vulkan_api;
    };
}
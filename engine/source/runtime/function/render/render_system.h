#pragma once

#include "runtime/function/render/render_pipeline_base.h"

#include <memory>

/**
 * render core
 */
namespace VKernel
{
    class VulkanAPI;
    class WindowSystem;

    class RenderSystem ///< render core
    {

    public:
        RenderSystem() = default; ///< Constructor
        ~RenderSystem(); ///< Destructor

        void initialize(std::shared_ptr<WindowSystem> window_system); ///< init
        void tick(float delta_time); ///< tick
        void clear(); ///< clear

        std::shared_ptr<VulkanAPI> getVulkanAPI() const;

    private:
        // direct management
        std::shared_ptr<VulkanAPI> m_vulkan_api;
        std::shared_ptr<RenderPipelineBase> m_render_pipeline;
    };
}
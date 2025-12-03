#pragma once

#include "runtime/function/render/render_pipeline_base.h"
#include "runtime/function/render/render_swap_context.h"

#include <memory>

/**
 * render core
 */
namespace VKernel
{
    class VulkanAPI;
    class WindowSystem;
    class RenderCamera;
    class RenderResourceBase;

    class RenderSystem ///< render core
    {

    public:
        RenderSystem() = default; ///< Constructor
        ~RenderSystem();          ///< Destructor

        void initialize(std::shared_ptr<WindowSystem> window_system); ///< init
        void tick(float delta_time);                                  ///< tick
        void clear();                                                 ///< clear

        // get
        std::shared_ptr<VulkanAPI> getVulkanAPI() const;
        RenderSwapContext &getSwapContext();
        std::shared_ptr<RenderCamera> getRenderCamera() const;

        void swapLogicRenderData(); ///< swap Logic Render Data

    private:
        // direct management
        std::shared_ptr<VulkanAPI> m_vulkan_api;
        std::shared_ptr<RenderCamera> m_render_camera;
        std::shared_ptr<RenderResourceBase> m_render_resource;
        std::shared_ptr<RenderPipelineBase> m_render_pipeline;
        RenderSwapContext m_swap_context;

    private:
        void processSwapData(); ///< Read data from the renderData of swapContext and pass it to each sub-module of the render process.
    };
}
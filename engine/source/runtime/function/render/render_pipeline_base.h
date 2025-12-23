#pragma once

#include "runtime/function/render/render_pass_base.h"
#include "runtime/function/render/vulkan_interface/vulkan_api.h"

/**
 * The management of each pass, the core of draw, abstract base class
 */
namespace VKernel
{
    class RenderResourceBase;
    class WindowUI;

    struct RenderPipelineInitInfo
    {
        std::shared_ptr<RenderResourceBase> render_resource;
    };

    class RenderPipelineBase
    {

        friend class RenderSystem;

    public:
        virtual ~RenderPipelineBase() {}                               ///< Constructor
        virtual void clear() {};                                       ///< clear
        virtual void initialize(RenderPipelineInitInfo init_info) = 0; ///< init

        virtual void preparePassData(std::shared_ptr<RenderResourceBase> render_resource); ///< prepare processing data

        virtual void forwardRender(std::shared_ptr<VulkanAPI>          vulkan_api,
                                   std::shared_ptr<RenderResourceBase> render_resource) = 0; ///< forward render core

        void initializeUIRenderBackend(WindowUI* window_ui); ///< init editor ui Backend: vulkan and glfw

    protected:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface

        // pass
        std::shared_ptr<RenderPassBase> m_main_camera_pass;
        std::shared_ptr<RenderPassBase> m_ui_pass;
        std::shared_ptr<RenderPassBase> m_combine_ui_pass;
    };
} // namespace VKernel
#pragma once

#include <memory>

/**
 * renderpass related, basic class
 */
namespace VKernel
{
    class VulkanAPI;
    class RenderResourceBase;
    class WindowUI;

    struct RenderPassInitInfo ///< The base class is empty, and the derived class has its own implementation
    {};

    struct RenderPassCommonInfo ///< Group info
    {
        std::shared_ptr<VulkanAPI>          vulkan_api;
        std::shared_ptr<RenderResourceBase> render_resource;
    };

    class RenderPassBase
    {
    public:
        virtual void initialize(const RenderPassInitInfo* init_info) = 0; ///< init
        virtual void postInitialize();                                    ///< post init

        virtual void setCommonInfo(RenderPassCommonInfo common_info);
        virtual void preparePassData(std::shared_ptr<RenderResourceBase> render_resource);
        virtual void initializeUIRenderBackend(WindowUI* window_ui); ///< init editor ui Backend: vulkan and glfw

    protected:
        std::shared_ptr<VulkanAPI>          m_vulkan_api;
        std::shared_ptr<RenderResourceBase> m_render_resource;
    };
} // namespace VKernel
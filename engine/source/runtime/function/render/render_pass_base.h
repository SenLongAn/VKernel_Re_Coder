#pragma once

#include <memory>

/**
 * renderpass related, basic class
 */
namespace VKernel
{
    class VulkanAPI;
    class RenderResourceBase;

    struct RenderPassCommonInfo
    {
        std::shared_ptr<VulkanAPI>          vulkan_api;
        std::shared_ptr<RenderResourceBase> render_resource;
    };

    class RenderPassBase
    {
    public:
        virtual void initialize() = 0;
        virtual void setCommonInfo(RenderPassCommonInfo common_info);
        virtual void preparePassData(std::shared_ptr<RenderResourceBase> render_resource);

    protected:
        std::shared_ptr<VulkanAPI>          m_vulkan_api;
        std::shared_ptr<RenderResourceBase> m_render_resource;
    };
} // namespace VKernel
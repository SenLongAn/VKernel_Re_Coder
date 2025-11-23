#pragma once

#include "runtime/function/render/render_resource_base.h"
#include "runtime/function/render/vulkan_interface/vulkan_api.h"

/**
 * The management of each pass, the core of draw, abstract base class
 */
namespace VKernel
{
    class RenderPipelineBase
    {

    public:
        virtual ~RenderPipelineBase() {} ///< Constructor
        virtual void clear() {}; ///< clear
        virtual void initialize() = 0; ///< init

        virtual void preparePassData(std::shared_ptr<RenderResourceBase> render_resource);
        virtual void forwardRender(std::shared_ptr<VulkanAPI> vulkan_api, std::shared_ptr<RenderResourceBase> render_resource); ///< forward render core

    protected:
        std::shared_ptr<VulkanAPI> m_vulkan_api;
    };
}
#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"

/**
 * The management of each pass, the core of draw, abstract base class
 */
namespace VKernel
{
    class RenderResourceBase;

    class RenderPipelineBase
    {

        friend class RenderSystem;
        
    public:
        virtual ~RenderPipelineBase() {} ///< Constructor
        virtual void clear() {}; ///< clear
        virtual void initialize() = 0; ///< init

        virtual void preparePassData(std::shared_ptr<RenderResourceBase> render_resource); ///< prepare processing data

        virtual void forwardRender(std::shared_ptr<VulkanAPI> vulkan_api) = 0; ///< forward render core

    protected:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface
    };
}
#pragma once

#include "runtime/function/render/render_pipeline_base.h"

/**
 * The management of each pass, the core of draw
 */
namespace VKernel
{
    class RenderPipeline : public RenderPipelineBase
    {

    public:
        virtual void initialize() override final {}

        virtual void clear() {};

        virtual void forwardRender(std::shared_ptr<VulkanAPI> vulkan_api);

        void passUpdateAfterRecreateSwapchain(); ///< destory and recreate framebuffer
    };
}
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
        virtual void initialize(RenderPipelineInitInfo init_info) override final;

        virtual void clear() {};

        virtual void forwardRender(std::shared_ptr<VulkanAPI>          vulkan_api,
                                   std::shared_ptr<RenderResourceBase> render_resource);

        void passUpdateAfterRecreateSwapchain(); ///< destory and recreate framebuffer

        virtual uint32_t getGuidOfPickedMesh(const Vector2& picked_uv) override final;
    };
} // namespace VKernel
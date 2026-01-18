#pragma once

#include "runtime/function/render/render_pass.h"

namespace VKernel
{
    struct ToneMappingPassInitInfo : RenderPassInitInfo
    {
        VkRenderPass render_pass;
        VkImageView  input_attachment;
    };

    class ToneMappingPass : public RenderPass
    {
    public:
        // init
        void initialize(const RenderPassInitInfo* init_info) override final;

        // draw
        void draw() override final;

        // init
        void updateAfterFramebufferRecreate(VkImageView input_attachment);

    private:
        // init
        void setupDescriptorSetLayout();
        void setupPipelines();
        void setupDescriptorSet();
    };
} // namespace VKernel

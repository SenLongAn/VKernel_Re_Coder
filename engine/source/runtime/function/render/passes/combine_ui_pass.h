#pragma once

#include "runtime/function/render/render_pass.h"

/**
 * combine secne and editorUI
 */

namespace VKernel
{
    struct CombineUIPassInitInfo : RenderPassInitInfo ///< info struct
    {
        VkRenderPass render_pass;
        VkImageView  scene_input_attachment; ///< scene
        VkImageView  ui_input_attachment;    ///< editorUI
    };

    class CombineUIPass : public RenderPass
    {
    public:
        void initialize(const RenderPassInitInfo* init_info) override final; ///< init
        void draw() override final;                                          ///< draw
        void updateAfterFramebufferRecreate(VkImageView scene_input_attachment,
                                            VkImageView ui_input_attachment); ///< reset bind image view and descriptor

    private:
        // init
        void setupDescriptorSetLayout();
        void setupPipelines();
        void setupDescriptorSet();
    };
} // namespace VKernel

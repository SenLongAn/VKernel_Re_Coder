#pragma once

#include "runtime/function/render/render_pass.h"

/**
 * Editor UI render subpass
 */
namespace VKernel
{
    class WindowUI;

    struct UIPassInitInfo : RenderPassInitInfo
    {
        VkRenderPass render_pass; ///< render pass
    };

    class UIPass : public RenderPass
    {
    public:
        void initialize(const RenderPassInitInfo* init_info) override final; ///< init
        void initializeUIRenderBackend() override final;                     ///< Integration of ImGui, Vulkan and GLFW
        void draw() override final;                                          ///< render

    private:
        void uploadFonts(); ///< upload font
    };
} // namespace VKernel

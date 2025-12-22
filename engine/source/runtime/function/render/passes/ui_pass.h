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
        void initializeUIRenderBackend(WindowUI* window_ui) override final;  ///< Integration of ImGui, Vulkan and GLFW
        void draw() override final;                                          ///< render

    private:
        WindowUI* m_window_ui; ///< Editor UI Settings

    private:
        void uploadFonts(); ///< upload font
    };
} // namespace VKernel

#include "runtime/Games/the_celestial_console/Panels/setting_panel.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/Games/the_celestial_console/Panels/main_panel.h"

namespace Games
{
    WINDOWUI_REGISTER(SettingPanel, true);

    void SettingPanel::initialize(VKernel::WindowUIInitInfo init_info) {}

    void SettingPanel::preRender()
    {

        if (!MainPanel::isPanelOpen)
        {
            return;
        }

        if (MainPanel::isPanelOne)
        {
            return;
        }

        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;

        ImGui::SetNextWindowPos(
            ImVec2(viewport.x + viewport.width - viewport.width / 10.0f * 2 + viewport.width / 100.0f, viewport.y),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width / 10.0f * 2, viewport.height), ImGuiCond_Always);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

        ImGui::Begin("SETTING", nullptr, window_flags);

        // Automatically adjust font size
        float windowArea = ImGui::GetWindowSize().x * ImGui::GetWindowSize().y;
        float scale = sqrtf(windowArea / (640 * 0.20 * 400)) * 0.5;
        ImGui::SetWindowFontScale(scale);

        if (isMouseInWindowRange())
        {
            selected_object = false;
        }

        ImGui::End();
    }
} // namespace Games
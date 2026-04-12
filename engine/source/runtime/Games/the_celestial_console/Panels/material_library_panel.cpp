#include "runtime/Games/the_celestial_console/Panels/material_library_panel.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/Games/the_celestial_console/Panels/main_panel.h"
#include "runtime/core/base/macro.h"
#include "material_library_panel.h"

namespace Games
{
    WINDOWUI_REGISTER(MaterialLibraryPanel, true);

    void MaterialLibraryPanel::initialize(VKernel::WindowUIInitInfo init_info) {}

    void MaterialLibraryPanel::preUpdate()
    {
    }

    void MaterialLibraryPanel::preRender()
    {

        if (!MainPanel::isPanelOpen)
        {
            return;
        }

        if (!MainPanel::isPanelOne)
        {
            return;
        }

        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;

        ImGui::SetNextWindowPos(
            ImVec2(viewport.x, viewport.y),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width / 10.0f * 2, viewport.height), ImGuiCond_Always);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

        ImGui::Begin(ICON_FA_ANCHOR " MATERIAL LIBRARY", nullptr, window_flags);

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
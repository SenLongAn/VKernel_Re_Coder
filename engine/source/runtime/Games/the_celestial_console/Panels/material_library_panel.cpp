#include "runtime/Games/the_celestial_console/Panels/material_library_panel.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/Games/the_celestial_console/Panels/main_panel.h"
#include "runtime/core/base/macro.h"

namespace Games
{
    WINDOWUI_REGISTER(MaterialLibraryPanel, true);

    void MaterialLibraryPanel::initialize(VKernel::WindowUIInitInfo init_info) {}

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

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.002f, 0.0f, 0.0f, 1.0f));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

        bool isOpen = true;
        ImGui::Begin("MaterialLibraryPanel", &isOpen, window_flags);

        // Automatically adjust font size
        float windowArea = ImGui::GetWindowSize().x * ImGui::GetWindowSize().y;
        float scale = sqrtf(windowArea / (640 * 0.20 * 400)) * 0.5;
        ImGui::SetWindowFontScale(scale);

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "MaterialLibrary Panel");

        if (ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered())
        {
            selected_object = false;
        }

        ImGui::End();

        ImGui::PopStyleColor();
    }
} // namespace Games
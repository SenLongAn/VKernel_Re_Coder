#include "runtime/Games/the_celestial_console/Panels/main_panel.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "main_panel.h"
#include <vulkan/vulkan.h>

namespace Games
{
    WINDOWUI_REGISTER(MainPanel, true);

    void MainPanel::initialize(VKernel::WindowUIInitInfo init_info)
    {
        // set imgui state
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; ///< Enable docking feature
        io.ConfigDockingAlwaysTabBar         = true;      ///< Always show the title bar
        io.ConfigWindowsMoveFromTitleBarOnly = true;      ///< Window can only be dragged from the title bar
        io.IniFilename                       = nullptr;   ///< Do not save the layout
        io.FontGlobalScale                   = 2.0f;
    }

    void MainPanel::preRender()
    {
        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;

        ImGui::SetNextWindowPos(ImVec2(viewport.x, viewport.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width, viewport.height), ImGuiCond_Always);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
        bool isOpen = true;
        ImGui::Begin("new window", &isOpen, window_flags);

        ImVec2 buttonSize(150, 30);
        ImVec2 buttonPos(viewport.width / 2.0f - buttonSize.x / 2.0f, viewport.height / 100.0f);
        ImGui::SetCursorPos(buttonPos);
        if (!isPanelOpen)
        {
            if (ImGui::Button("OpenPanel", buttonSize))
            {
                isPanelOpen = !isPanelOpen;
            }
        }
        else
        {
            if (ImGui::Button("ClosePanel", buttonSize))
            {
                isPanelOpen = !isPanelOpen;
            }
        }

        ImGui::End();

        DrawCrosshair();
    }

    void Games::MainPanel::DrawCrosshair()
    {
        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;

        ImGui::SetNextWindowPos(ImVec2(viewport.x, viewport.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width, viewport.height), ImGuiCond_Always);

        ImGuiWindowFlags flags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoResize;

        ImGui::Begin("Crosshair", nullptr, flags);

        DrawCrosshairShape();

        ImGui::End();
    }

    void MainPanel::DrawCrosshairShape()
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        VkViewport  viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;
        ImVec2 center = {viewport.x + viewport.width / 2.0f, viewport.y + viewport.height / 2.0f};

        float length    = 15.0f;
        float gap       = 5.0f;
        float thickness = 2.0f;
        ImU32 color     = IM_COL32(255, 0, 0, 200);

        draw_list->AddLine(ImVec2(center.x, center.y - length), ImVec2(center.x, center.y - gap), color, thickness);

        draw_list->AddLine(ImVec2(center.x, center.y + gap), ImVec2(center.x, center.y + length), color, thickness);

        draw_list->AddLine(ImVec2(center.x - length, center.y), ImVec2(center.x - gap, center.y), color, thickness);

        draw_list->AddLine(ImVec2(center.x + gap, center.y), ImVec2(center.x + length, center.y), color, thickness);
    }
} // namespace Games

#include "runtime/Games/the_celestial_console/Panels/main_panel.h"

#include "runtime/core/log/log_system.h"
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

        ImGui::SetNextWindowPos(ImVec2(viewport.x + viewport.width / 10.0f * 2, viewport.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width / 10.0f * 6, viewport.height), ImGuiCond_Always);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
        bool isOpen = true;
        ImGui::Begin("MainPanel", &isOpen, window_flags);

        // text:
        ImGui::SetCursorPosX(viewport.width / 10.0f * 1);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Direction:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(viewport.width / 10.0f * 2);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Temperature:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(viewport.width / 10.0f * 3);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Sunlight:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(viewport.width / 10.0f * 4);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "FrameRate:");
        ImGui::SameLine();

        // button
        ImVec2 buttonSize(40, 30);
        ImVec2 buttonPos(viewport.width / 10.0F * 5, viewport.height / 100.0f);
        ImGui::SetCursorPos(buttonPos);
        if (!isPanelOpen)
        {
            if (ImGui::Button("<>", buttonSize))
            {
                isPanelOpen = !isPanelOpen;
            }
        }
        else
        {
            if (ImGui::Button("><", buttonSize))
            {
                isPanelOpen = !isPanelOpen;
            }
        }

        // Crosshair
        DrawCrosshairShape();

        // log
        ImGui::SetCursorPos(ImVec2(viewport.width / 10.0f * 1.5, viewport.height - viewport.height / 10.0f * 1.4));

        ImGui::BeginChild("LogRegion", ImVec2(0, 150), false);

        std::vector<VKernel::LogMessage> messages = VKernel::g_runtime_global_context.m_logger_system->GetMessage();
        for (const auto& log : messages)
        {
            switch (log.level)
            {
                case VKernel::LogLevel::debug:
                    ImGui::TextColored(ImVec4(0.1f, 0.0f, 1.0f, 1.0f), "%s", log.log.c_str());
                    break;
                case VKernel::LogLevel::info:
                    ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.0f, 1.0f), "%s", log.log.c_str());
                    break;
                case VKernel::LogLevel::warn:
                    ImGui::TextColored(ImVec4(0.1f, 0.1f, 0.1f, 1.0f), "%s", log.log.c_str());
                    break;
                case VKernel::LogLevel::error:
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", log.log.c_str());
                    break;
                default:
                    break;
            }
        }

        ImGui::SetScrollHereY(0.0f);

        ImGui::EndChild();

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

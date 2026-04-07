#include "runtime/Games/the_celestial_console/Panels/main_panel.h"

#include "runtime/core/log/log_system.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"

#include <vulkan/vulkan.h>

namespace Games
{
    bool MainPanel::isPanelOpen = true;
    bool MainPanel::isPanelOne = true;

    WINDOWUI_REGISTER(MainPanel, true);

    void MainPanel::initialize(VKernel::WindowUIInitInfo init_info)
    {
    }

    void MainPanel::preRender()
    {
        selected_object = true;

        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;

        ImGui::SetNextWindowPos(ImVec2(viewport.x + viewport.width / 10.0f * 2, viewport.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width / 10.0f * 6, viewport.height), ImGuiCond_Always);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        bool isOpen = true;
        ImGui::Begin("MainPanel", &isOpen, window_flags);

        // Automatically adjust font size
        float windowArea = ImGui::GetWindowSize().x * ImGui::GetWindowSize().y;
        float scale = sqrtf(windowArea / (640 * 0.20 * 400)) * 0.3;
        ImGui::SetWindowFontScale(scale);

        // text:
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 10.0f * 1);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Direction:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 10.0f * 3);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Temperature:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 10.0f * 5);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Sunlight:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 10.0f * 7);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "FrameRate:");
        ImGui::SameLine();

        // button
        ImVec2 buttonSize(ImGui::GetWindowSize().x / 50.0f, ImGui::GetWindowSize().y / 50.0f);
        ImVec2 buttonPos(ImGui::GetWindowSize().x / 10.0F * 9, viewport.height / 100.0f);
        ImGui::SetCursorPos(buttonPos);
        if (isPanelOne)
        {
            if (ImGui::Button("1", buttonSize)) ///< Current status
            {
                isPanelOne = !isPanelOne;
            }
        }
        else
        {
            if (ImGui::Button("2", buttonSize))
            {
                isPanelOne = !isPanelOne;
            }
        }

        ImVec2 buttonSize1(ImGui::GetWindowSize().x / 50.0f, ImGui::GetWindowSize().y / 50.0f);
        ImVec2 buttonPos1(ImGui::GetWindowSize().x / 10.0F * 9.2, viewport.height / 100.0f);
        ImGui::SetCursorPos(buttonPos1);
        if (isPanelOpen)
        {
            if (ImGui::Button("<>", buttonSize1))
            {
                isPanelOpen = !isPanelOpen;
            }
        }
        else
        {
            if (ImGui::Button("><", buttonSize1))
            {
                isPanelOpen = !isPanelOpen;
            }
        }

        // Crosshair
        DrawCrosshairShape();

        // log
        float curY = ImGui::GetWindowSize().y;
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 10.0f * 2.5, curY - curY / 10.0f * 1.5));

        ImGui::BeginChild("LogRegion", ImVec2(0, curY / 10.0f * 1.4), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        std::vector<VKernel::LogMessage> messages = VKernel::g_runtime_global_context.m_logger_system->GetMessage();
        for (const auto &log : messages)
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

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();

        ImGui::End();
    }

    void MainPanel::DrawCrosshairShape()
    {
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;
        ImVec2 center = {viewport.x + viewport.width / 2.0f, viewport.y + viewport.height / 2.0f};

        float length = viewport.x / 5.0f;
        float gap = viewport.y / 5.0f;
        float thickness = viewport.x / 5.0f;
        ImU32 color = IM_COL32(255, 0, 0, 200);

        draw_list->AddLine(ImVec2(center.x, center.y - length), ImVec2(center.x, center.y - gap), color, thickness);

        draw_list->AddLine(ImVec2(center.x, center.y + gap), ImVec2(center.x, center.y + length), color, thickness);

        draw_list->AddLine(ImVec2(center.x - length, center.y), ImVec2(center.x - gap, center.y), color, thickness);

        draw_list->AddLine(ImVec2(center.x + gap, center.y), ImVec2(center.x + length, center.y), color, thickness);
    }
} // namespace Games

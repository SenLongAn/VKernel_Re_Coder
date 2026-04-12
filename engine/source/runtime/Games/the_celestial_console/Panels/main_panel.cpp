#include "runtime/Games/the_celestial_console/Panels/main_panel.h"

#include "runtime/core/log/log_system.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/engine.h"

#include <vulkan/vulkan.h>

namespace Games
{
    bool MainPanel::isPanelOpen = true;
    bool MainPanel::isPanelOne = true;

    WINDOWUI_REGISTER(MainPanel, true);

    void MainPanel::initialize(VKernel::WindowUIInitInfo init_info)
    {
    }

    void MainPanel::preUpdate()
    {
        if (VKernel::g_is_update_mode)
        {
            updateFont("asset/font/DigitalNumbers.ttf", 10);
            VKernel::g_is_update_mode = false;
        }
    }
    void MainPanel::preRender()
    {
        setUIColorStyle();

        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;

        ImGui::SetNextWindowPos(ImVec2(viewport.x + viewport.width / 10.0f * 2, viewport.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width / 10.0f * 6, viewport.height), ImGuiCond_Always);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

        float original_border_size = ImGui::GetStyle().WindowBorderSize;
        ImGui::GetStyle().WindowBorderSize = 0.0f;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        bool isOpen = true;
        ImGui::Begin("MainPanel", &isOpen, window_flags);

        // Automatically adjust font size
        float windowArea = ImGui::GetWindowSize().x * ImGui::GetWindowSize().y;
        float scale = sqrtf(windowArea / (640 * 0.20 * 400)) * 0.3;
        ImGui::SetWindowFontScale(scale);

        // button
        ImVec2 buttonSize(ImGui::GetWindowSize().x / 10.0f, ImGui::GetWindowSize().y / 50.0f);
        ImVec2 buttonPos(ImGui::GetWindowSize().x / 10.0F * 2.5, viewport.height / 100.0f);
        ImGui::SetCursorPos(buttonPos);
        if (isPanelOne)
        {
            if (ImGui::Button("BUILD MODE", buttonSize)) ///< Current status
            {
                isPanelOne = !isPanelOne;
            }
        }
        else
        {
            if (ImGui::Button("GALAXY MODE", buttonSize))
            {
                isPanelOne = !isPanelOne;
            }
        }

        ImVec2 buttonSize1(ImGui::GetWindowSize().x / 10.0f, ImGui::GetWindowSize().y / 50.0f);
        ImVec2 buttonPos1(ImGui::GetWindowSize().x / 10.0F * 6.5, viewport.height / 100.0f);
        ImGui::SetCursorPos(buttonPos1);
        if (isPanelOpen)
        {
            if (ImGui::Button("OPEN CONTROLE", buttonSize1))
            {
                isPanelOpen = !isPanelOpen;
            }
        }
        else
        {
            if (ImGui::Button("CLOSE CONTROLE", buttonSize1))
            {
                isPanelOpen = !isPanelOpen;
            }
        }

        // Crosshair
        DrawCrosshairShape();

        ImGui::End();

        ImGui::GetStyle().WindowBorderSize = original_border_size;

        ImGui::PopStyleColor();
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
        ImU32 color = IM_COL32(255, 0, 255, 200);

        draw_list->AddLine(ImVec2(center.x, center.y - length), ImVec2(center.x, center.y - gap), color, thickness);

        draw_list->AddLine(ImVec2(center.x, center.y + gap), ImVec2(center.x, center.y + length), color, thickness);

        draw_list->AddLine(ImVec2(center.x - length, center.y), ImVec2(center.x - gap, center.y), color, thickness);

        draw_list->AddLine(ImVec2(center.x + gap, center.y), ImVec2(center.x + length, center.y), color, thickness);
    }

    void MainPanel::setUIColorStyle()
    {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.0f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.50f, 1.00f);

        colors[ImGuiCol_WindowBg] = ImVec4(0.01f, 0.01f, 0.03f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.03f, 0.02f, 0.06f, 0.80f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.05f, 0.15f, 0.95f);

        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.80f, 1.00f, 0.60f);
        colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 0.00f, 0.80f, 0.30f);

        colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.05f, 0.15f, 0.90f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.05f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.10f, 0.30f, 1.00f);

        colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.04f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.05f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.06f, 0.03f, 0.10f, 0.80f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.03f, 0.10f, 1.00f);

        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.02f, 0.08f, 0.80f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 0.00f, 0.80f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.20f, 0.90f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.80f, 1.00f, 1.00f);

        colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.00f, 0.80f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.80f, 1.00f, 1.00f);

        colors[ImGuiCol_Button] = ImVec4(0.08f, 0.04f, 0.15f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.80f, 1.00f, 0.70f);
        colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.00f, 0.80f, 0.80f);

        colors[ImGuiCol_Header] = ImVec4(0.12f, 0.05f, 0.20f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.60f, 0.80f, 0.60f);
        colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.00f, 0.60f, 0.60f);

        colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.80f, 1.00f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.00f, 0.80f, 0.80f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 1.00f, 0.80f, 1.00f);

        colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.00f, 0.80f, 0.40f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.80f, 1.00f, 0.80f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 0.80f, 1.00f);

        colors[ImGuiCol_Tab] = ImVec4(0.06f, 0.03f, 0.12f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.70f, 0.90f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.10f, 0.05f, 0.20f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.04f, 0.02f, 0.08f, 0.80f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.08f, 0.04f, 0.15f, 0.90f);

        colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.80f, 1.00f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.02f, 0.01f, 0.04f, 1.00f);

        colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 0.80f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.80f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.80f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.80f, 1.00f, 1.00f);

        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.08f, 0.04f, 0.15f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.80f, 1.00f, 0.80f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(1.00f, 0.00f, 0.80f, 0.40f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.80f, 0.00f, 1.00f, 0.08f);

        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.80f, 1.00f, 0.40f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.00f, 0.80f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 1.00f, 0.80f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.80f, 0.80f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.80f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
    }
} // namespace Games

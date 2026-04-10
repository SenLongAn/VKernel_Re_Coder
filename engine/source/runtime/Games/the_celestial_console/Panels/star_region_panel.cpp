#include "runtime/Games/the_celestial_console/Panels/star_region_panel.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/Games/the_celestial_console/Panels/main_panel.h"
#include "runtime/function/render/render_resource.h"

#include <string>

namespace Games
{
    WINDOWUI_REGISTER(StarRegionPanel, true);

    void StarRegionPanel::initialize(VKernel::WindowUIInitInfo init_info) {}

    void StarRegionPanel::preRender()
    {

        // Is it render
        if (!MainPanel::isPanelOpen)
        {
            return;
        }

        if (MainPanel::isPanelOne)
        {
            return;
        }

        // window
        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;

        ImGui::SetNextWindowPos(
            ImVec2(viewport.x, viewport.y),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width / 10.0f * 2, viewport.height), ImGuiCond_Always);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.03f, 0.03f, 0.04f, 1.0f));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        bool isOpen = true;
        ImGui::Begin("StarRegionPanel", &isOpen, window_flags);

        ImGui::GetStyle().WindowPadding = ImVec2(0, 0);

        float window_x = ImGui::GetWindowSize().x;

        // Automatically adjust font size
        float windowArea = ImGui::GetWindowSize().x * ImGui::GetWindowSize().y;
        float scale = sqrtf(windowArea / (640 * 0.20 * 400)) * 0.5;
        ImGui::SetWindowFontScale(scale);

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "StarRegion Panel");

        // Whether the panel can be penetrated
        if (isMouseInWindowRange())
        {
            selected_object = false;
        }

        // Scrollable window
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 4));

        ImGui::BeginChild("Planets", ImVec2(ImGui::GetWindowSize().x + ImGui::GetWindowSize().x * 0.05, (ImGui::GetWindowSize().y / 4) * 2.8), window_flags);

        // Whether the panel can be penetrated
        if (isMouseInWindowRange())
        {
            selected_object = false;
        }

        // button
        static bool isFirst = true;
        if (isFirst)
        {
            createImGuiDescriptorSetForTexture(image, image_view, image_allocation, descriptorSetLayout, descriptorSet, texture_id,
                                               "asset/objects/_textures/gun.jpg", true);
            isFirst = false;
        }

        int col = 3;
        float spacing = window_x * 0.035;
        ImVec2 buttonSize((window_x - spacing) / col, (window_x - spacing) / col);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((spacing / 6.0f), (spacing / 6.0f)));

        for (int i = 0; i < 20; i++)
        {
            if ((i % col) != 0)
                ImGui::SameLine(0, 0);

            ImGui::ImageButton(texture_id, buttonSize);
        }

        ImGui::PopStyleVar(3);

        ImGui::EndChild();

        ImGui::End();

        ImGui::PopStyleColor();
    }
} // namespace Games
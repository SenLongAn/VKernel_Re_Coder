#include "runtime/Games/the_celestial_console/Panels/main_panel.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <vulkan/vulkan.h>

namespace Games
{
    WINDOWUI_REGISTER(MainPanel);

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
    }
} // namespace Games
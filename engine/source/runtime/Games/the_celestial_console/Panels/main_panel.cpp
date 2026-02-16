#include "runtime/Games/the_celestial_console/Panels/main_panel.h"

#include <imgui.h>
#include <imgui_internal.h>

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
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        bool             isOpen       = true;
        ImGui::Begin("new window", &isOpen, window_flags);
        ImGui::Button("hello");
        ImGui::End();
    }
} // namespace Games
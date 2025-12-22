#include "editor/include/editor_ui.h"

#include "runtime/function/render/render_system.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace ReCoder
{
    void EditorUI::initialize(VKernel::WindowUIInitInfo init_info)
    {
        // create imgui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // initialize imgui vulkan render backend
        init_info.render_system->initializeUIRenderBackend(this);
    }

    void EditorUI::preRender() { showEditorUI(); }

    void EditorUI::showEditorUI() { showEditorGameWindow(&m_game_engine_window_open); }

    void EditorUI::showEditorGameWindow(bool* p_open)
    {
        if (!*p_open)
            return;

        // begin window
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar; ///< Window icon: no background, includes menu bar
        if (!ImGui::Begin("Game Engine", p_open, window_flags))
        {
            ImGui::End();
            return;
        }
        ImGui::Button("Hello World!");

        ImGui::End(); ///< end window
    }

} // namespace ReCoder
#include "editor/include/editor_ui.h"

#include "runtime/function/global/global_context.h"
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

        // Calculate render area size
        VKernel::Vector2 render_target_window_pos  = {0.0f, 0.0f};
        VKernel::Vector2 render_target_window_size = {0.0f, 0.0f};

        auto menu_bar_rect = ImGui::GetCurrentWindow()->MenuBarRect();

        render_target_window_pos.x  = ImGui::GetWindowPos().x;
        render_target_window_pos.y  = menu_bar_rect.Max.y;
        render_target_window_size.x = ImGui::GetWindowSize().x;
        render_target_window_size.y = (ImGui::GetWindowSize().y + ImGui::GetWindowPos().y) - menu_bar_rect.Max.y;

        VKernel::g_runtime_global_context.m_render_system->updateEngineContentViewport(render_target_window_pos.x,
                                                                                       render_target_window_pos.y,
                                                                                       render_target_window_size.x,
                                                                                       render_target_window_size.y);

        ImGui::End(); ///< end window
    }

} // namespace ReCoder
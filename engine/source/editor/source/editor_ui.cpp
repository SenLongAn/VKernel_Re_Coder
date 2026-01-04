#include "editor/include/editor_ui.h"

#include "editor/include/editor_global_context.h"

#include "runtime/engine.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <array>

namespace ReCoder
{
    void EditorUI::initialize(VKernel::WindowUIInitInfo init_info)
    {
        // create imgui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // set imgui state
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; ///< Enable docking feature
        io.ConfigDockingAlwaysTabBar         = true;      ///< Always show the title bar
        io.ConfigWindowsMoveFromTitleBarOnly = true;      ///< Window can only be dragged from the title bar
        io.IniFilename                       = nullptr;   ///< Do not save the layout
        io.FontGlobalScale                   = 2.0f;

        // set color style
        setUIColorStyle();

        // initialize imgui vulkan render backend
        init_info.render_system->initializeUIRenderBackend(this);
    }

    void EditorUI::preRender() { showEditorUI(); }

    void EditorUI::showEditorUI()
    {
        // Render 5 windows
        showEditorMenu(&m_editor_menu_window_open);
        showEditorWorldObjectsWindow(&m_asset_window_open);
        showEditorGameWindow(&m_game_engine_window_open);
        showEditorFileContentWindow(&m_file_content_window_open);
        showEditorDetailWindow(&m_detail_window_open);
    }

    void EditorUI::showEditorMenu(bool* p_open)
    {
        // create menu window

        // Set window position and size to fill the viewport
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(main_viewport->WorkPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(main_viewport->WorkSize, ImGuiCond_Always);
        ImGui::SetNextWindowViewport(main_viewport->ID);

        // create window
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_MenuBar |    // Includes menu bar
            ImGuiWindowFlags_NoTitleBar | // No title bar: at the top of the window, shows window name, includes actions
                                          // (minimize, maximize, close)
            ImGuiWindowFlags_NoCollapse | // Cannot be collapsed
            ImGuiWindowFlags_NoResize |   // Cannot be resized
            ImGuiWindowFlags_NoMove |     // Cannot be moved
                                          // No background (transparent)
            ImGuiConfigFlags_NoMouseCursorChange |  // Does not change the mouse cursor
            ImGuiWindowFlags_NoBringToFrontOnFocus; // Does not stay on top when focused
        ImGui::Begin("Editor menu", p_open, window_flags);

        // create dockSapce

        ImGuiID main_docking_id = ImGui::GetID("Main Docking");    ///< get dockSpace id
        if (ImGui::DockBuilderGetNode(main_docking_id) == nullptr) ///< If it does not exist，initial
        {
            // remove old dockSpace
            ImGui::DockBuilderRemoveNode(main_docking_id);

            // add dockSpace node
            ImGuiDockNodeFlags dock_flags = ImGuiDockNodeFlags_DockSpace; ///< flag: Allow other windows to dock here
            ImGui::DockBuilderAddNode(main_docking_id, dock_flags);

            // set position and size
            ImGui::DockBuilderSetNodePos(
                main_docking_id,
                ImVec2(main_viewport->WorkPos.x,
                       main_viewport->WorkPos.y + 18.0f)); ///< Set position: move the Y coordinate down by 18 pixels
                                                           ///< (to make room for the menu bar)
            ImGui::DockBuilderSetNodeSize(main_docking_id,
                                          ImVec2(main_viewport->WorkSize.x,
                                                 main_viewport->WorkSize.y - 18.0f)); // set size

            // Window Dock Preset
            ImGuiID center = main_docking_id;
            ImGuiID left;
            ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.5f, nullptr, &left);

            ImGuiID left_other;
            ImGuiID left_file_content = ImGui::DockBuilderSplitNode(left, ImGuiDir_Down, 0.7f, nullptr, &left_other);

            ImGuiID left_game_engine;
            ImGuiID left_asset =
                ImGui::DockBuilderSplitNode(left_other, ImGuiDir_Left, 0.9f, nullptr, &left_game_engine);

            ImGui::DockBuilderDockWindow("World Objects", left_asset);
            ImGui::DockBuilderDockWindow("Components Details", right);
            ImGui::DockBuilderDockWindow("File Content", left_file_content);
            ImGui::DockBuilderDockWindow("Game Engine", left_game_engine);

            // finish
            ImGui::DockBuilderFinish(main_docking_id);
        }

        ImGui::DockSpace(main_docking_id); ///< render dockSapce

        // MenuBar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    g_editor_global_context.m_engine_runtime->shutdownEngine();
                    exit(0);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // close window
        ImGui::End();
    }

    void EditorUI::showEditorWorldObjectsWindow(bool* p_open)
    {
        // If the window is closed
        if (!*p_open)
            return;

        // Set the window background color based on game window
        if (m_on_game_window)
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        }

        else
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.002f, 0.0f, 0.0f, 1.0f));
        }

        // create window
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        if (!ImGui::Begin("World Objects", p_open, window_flags))
        {
            ImGui::End();
            ImGui::PopStyleColor();
            return;
        }

        // render button
        ImGui::Button("Hello World!");

        ImGui::End();

        ImGui::PopStyleColor();
    }

    void EditorUI::showEditorFileContentWindow(bool* p_open)
    {
        if (!*p_open)
            return;

        // Set the window background color based on game window
        if (m_on_game_window)
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        }

        else
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.002f, 0.0f, 0.0f, 1.0f));
        }

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        if (!ImGui::Begin("File Content", p_open, window_flags))
        {
            ImGui::End();
            ImGui::PopStyleColor();
            return;
        }

        ImGui::Button("Hello World!");

        ImGui::End();
        ImGui::PopStyleColor();
    }

    void EditorUI::showEditorGameWindow(bool* p_open)
    {
        if (!*p_open)
            return;

        // begin window
        ImGui::SetNextWindowBgAlpha(1.0f);                        // Make background transparent instead of NoBackground
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar; ///< Window icon: includes menu bar
        if (!ImGui::Begin("Game Engine", p_open, window_flags))
        {
            ImGui::End();
            return;
        }
        ImGui::Button("Hello World!");

        //
        if (ImGui::IsMouseClicked(0))
        {
            ImGuiWindow* window         = ImGui::GetCurrentWindow();
            ImRect       title_bar_rect = window->TitleBarRect();

            ImVec2 mouse_pos = ImGui::GetMousePos();
            m_on_game_window = title_bar_rect.Contains(mouse_pos);
        }

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

    void EditorUI::showEditorDetailWindow(bool* p_open)
    {
        if (!*p_open)
            return;

        // Set the window background color based on game window
        if (m_on_game_window)
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        }

        else
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.002f, 0.0f, 0.0f, 1.0f));
        }

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        if (!ImGui::Begin("Components Details", p_open, window_flags))
        {
            ImGui::End();
            ImGui::PopStyleColor();
            return;
        }

        ImGui::Button("Hello World!");

        ImGui::End();

        ImGui::PopStyleColor();
    }

    void EditorUI::setUIColorStyle()
    {
        ImGuiStyle* style  = &ImGui::GetStyle();
        ImVec4*     colors = style->Colors;

        colors[ImGuiCol_Text]                  = ImVec4(0.4745f, 0.4745f, 0.4745f, 1.00f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border]                = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.047f, 0.047f, 0.047f, 0.5411f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.196f, 0.196f, 0.196f, 0.40f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.294f, 0.294f, 0.294f, 0.67f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.0039f, 0.0039f, 0.0039f, 1.00f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.0039f, 0.0039f, 0.0039f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark]             = ImVec4(93.0f / 255.0f, 10.0f / 255.0f, 66.0f / 255.0f, 1.00f);
        colors[ImGuiCol_SliderGrab]            = colors[ImGuiCol_CheckMark];
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.3647f, 0.0392f, 0.2588f, 0.50f);
        colors[ImGuiCol_Button]                = ImVec4(0.0117f, 0.0117f, 0.0117f, 1.00f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.0235f, 0.0235f, 0.0235f, 1.00f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.0353f, 0.0196f, 0.0235f, 1.00f);
        colors[ImGuiCol_Header]                = ImVec4(0.1137f, 0.0235f, 0.0745f, 0.588f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(5.0f / 255.0f, 5.0f / 255.0f, 5.0f / 255.0f, 1.00f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
        colors[ImGuiCol_Separator]             = ImVec4(0.0f, 0.0f, 0.0f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(45.0f / 255.0f, 7.0f / 255.0f, 26.0f / 255.0f, 1.00f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(45.0f / 255.0f, 7.0f / 255.0f, 26.0f / 255.0f, 1.00f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab]                   = ImVec4(6.0f / 255.0f, 6.0f / 255.0f, 8.0f / 255.0f, 1.00f);
        colors[ImGuiCol_TabHovered]            = ImVec4(45.0f / 255.0f, 7.0f / 255.0f, 26.0f / 255.0f, 150.0f / 255.0f);
        colors[ImGuiCol_TabActive]             = ImVec4(47.0f / 255.0f, 6.0f / 255.0f, 29.0f / 255.0f, 1.0f);
        colors[ImGuiCol_TabUnfocused]          = ImVec4(45.0f / 255.0f, 7.0f / 255.0f, 26.0f / 255.0f, 25.0f / 255.0f);
        colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(6.0f / 255.0f, 6.0f / 255.0f, 8.0f / 255.0f, 200.0f / 255.0f);
        colors[ImGuiCol_DockingPreview]        = ImVec4(47.0f / 255.0f, 6.0f / 255.0f, 29.0f / 255.0f, 0.7f);
        colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(2.0f / 255.0f, 2.0f / 255.0f, 2.0f / 255.0f, 1.0f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 1.00f, 0.0f, 1.00f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 1.00f, 0.0f, 1.00f);
    }
} // namespace ReCoder
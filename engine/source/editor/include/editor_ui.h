#pragma once

#include "runtime/core/math/vector2.h"
#include "runtime/function/ui/window_ui.h"

/**
 * Editor UI Settings
 */

namespace VKernel
{
    class WindowUI;
}

namespace ReCoder
{
    class EditorUI : public VKernel::WindowUI
    {
    public:
        EditorUI() {}

        virtual void initialize(VKernel::WindowUIInitInfo init_info) override final; ///< init
        virtual void preRender() override final;                                     ///< render ui

    private:
        // window open or close
        bool m_editor_menu_window_open       = true;
        bool m_asset_window_open             = true;
        bool m_game_engine_window_open       = true;
        bool m_file_content_window_open      = true;
        bool m_detail_window_open            = true;
        bool m_scene_lights_window_open      = true;
        bool m_scene_lights_data_window_open = true;

        bool m_on_game_window = false;

    private:
        // show editor window
        void showEditorUI();

        void showEditorMenu(bool* p_open);
        void showEditorWorldObjectsWindow(bool* p_open);
        void showEditorFileContentWindow(bool* p_open);
        void showEditorGameWindow(bool* p_open);
        void showEditorDetailWindow(bool* p_open);

        // UI color
        void setUIColorStyle();
    };
} // namespace ReCoder
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

        virtual void initialize(VKernel::WindowUIInitInfo init_info) override final;
        virtual void preRender() override final;

    private:
        // window open or close
        bool m_game_engine_window_open = true;

    private:
        // show editor window
        void showEditorUI();

        void showEditorGameWindow(bool* p_open);
    };
} // namespace ReCoder
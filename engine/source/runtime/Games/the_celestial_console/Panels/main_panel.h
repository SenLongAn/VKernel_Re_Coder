#pragma once

#include "runtime/function/ui/window_ui.h"

#include <string>
#include <vector>

namespace Games
{
    class MainPanel : public VKernel::WindowUI
    {
        WINDOWUI_CLASS(MainPanel)
    public:
        bool isPanelOpen = false;

    public:
        MainPanel(bool isGameMode) { m_only_game_mode = isGameMode; }
        void initialize(VKernel::WindowUIInitInfo init_info) override;
        void preRender() override;

    private:
        void DrawCrosshairShape();
    };
} // namespace Games

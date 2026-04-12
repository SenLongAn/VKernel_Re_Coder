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
        static bool isPanelOpen;
        static bool isPanelOne;

    public:
        MainPanel(bool isGameMode) { m_only_game_mode = isGameMode; }
        void initialize(VKernel::WindowUIInitInfo init_info) override;
        virtual void preUpdate() override final; ///< pre render
        void preRender() override;

    private:
        void DrawCrosshairShape();
        void setUIColorStyle();
    };
} // namespace Games

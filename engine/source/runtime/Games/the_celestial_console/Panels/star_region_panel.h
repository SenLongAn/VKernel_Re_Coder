#pragma once

#include "runtime/function/ui/window_ui.h"

#include <string>
#include <vector>

namespace Games
{
    class StarRegionPanel : public VKernel::WindowUI
    {
        WINDOWUI_CLASS(StarRegionPanel)
    public:
        StarRegionPanel(bool isGameMode) { m_only_game_mode = isGameMode; }
        void initialize(VKernel::WindowUIInitInfo init_info) override;
        void preRender() override;

    private:
    };
} // namespace Games

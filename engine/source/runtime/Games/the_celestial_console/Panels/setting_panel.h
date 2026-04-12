#pragma once

#include "runtime/function/ui/window_ui.h"

#include <string>
#include <vector>

namespace Games
{
    class SettingPanel : public VKernel::WindowUI
    {
        WINDOWUI_CLASS(SettingPanel)
    public:
        SettingPanel(bool isGameMode) { m_only_game_mode = isGameMode; }
        void initialize(VKernel::WindowUIInitInfo init_info) override;
        virtual void preUpdate() override final; ///< pre render
        void preRender() override;

    private:
    };
} // namespace Games

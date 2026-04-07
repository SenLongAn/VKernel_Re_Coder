#pragma once

#include "runtime/function/ui/window_ui.h"

#include <string>
#include <vector>

namespace Games
{
    class MaterialLibraryPanel : public VKernel::WindowUI
    {
        WINDOWUI_CLASS(MaterialLibraryPanel)
    public:
        MaterialLibraryPanel(bool isGameMode) { m_only_game_mode = isGameMode; }
        void initialize(VKernel::WindowUIInitInfo init_info) override;
        void preRender() override;

    private:
    };
} // namespace Games

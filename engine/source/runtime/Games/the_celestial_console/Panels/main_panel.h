#pragma once

#include "runtime/function/ui/window_ui.h"

namespace Games
{
    class MainPanel : public VKernel::WindowUI
    {
        WINDOWUI_CLASS(MainPanel)
    public:
        void initialize(VKernel::WindowUIInitInfo init_info) override;
        void preRender() override;
    };
} // namespace Games

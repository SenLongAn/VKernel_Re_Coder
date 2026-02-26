#pragma once

#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/function/ui/window_ui.h"

#include <string>
#include <vector>

namespace Games
{
    class ControPanel : public VKernel::WindowUI
    {
        WINDOWUI_CLASS(ControPanel)
    public:
        ControPanel(bool isGameMode);
        void initialize(VKernel::WindowUIInitInfo init_info) override;
        void preRender() override;

    private:
        std::unordered_map<std::string, std::function<void(std::string, void*)>> m_functions;
        void createLeafNodeUI(VKernel::Reflection::ReflectionInstance& instance);
    };
} // namespace Games

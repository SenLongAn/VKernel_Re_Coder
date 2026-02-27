#include "runtime/function/ui/window_ui.h"
#include "runtime/engine.h"

namespace VKernel
{
    bool WindowUI::isPanelOpen     = false;
    bool WindowUI::selected_object = true;

    int WindowUIFactory::index = 0;

    void WindowUIManager::registerUIs() { m_uis = WindowUIFactory::getInstance().createUIs(); }

    void WindowUIManager::initUIs(WindowUIInitInfo init_info)
    {
        for (auto& ui : m_uis)
        {
            ui->initialize(init_info);
        }
    }

    void WindowUIManager::updateUIs()
    {
        for (auto& ui : m_uis)
        {
            if (VKernel::g_is_editor_mode && ui->iSGameMode())
                continue;
            ui->preRender();
        }
    }

    WindowUIManager::~WindowUIManager() { m_uis.clear(); }
} // namespace VKernel

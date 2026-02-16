#include "runtime/function/ui/window_ui.h"

namespace VKernel
{

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
            ui->preRender();
        }
    }

    WindowUIManager::~WindowUIManager() { m_uis.clear(); }
} // namespace VKernel

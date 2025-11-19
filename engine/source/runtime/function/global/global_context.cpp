#include "runtime/function/global/global_context.h"

#include "runtime/function/render/window_system.h"

namespace VKernel
{
    RuntimeGlobalContext g_runtime_global_context; ///< instance

    void RuntimeGlobalContext::startSystems()
    {
        m_window_system = std::make_shared<WindowSystem>();
        WindowCreateInfo window_create_info;
        m_window_system->initialize(window_create_info);
    }

    void RuntimeGlobalContext::shutdownSystems()
    {
        m_window_system.reset();
    }
}
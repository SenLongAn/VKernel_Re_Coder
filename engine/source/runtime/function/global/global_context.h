#pragma once

#include <memory>

namespace VKernel
{
    
    class WindowSystem;

    class RuntimeGlobalContext ///< Management system and manager
    {
        
    public:
        std::shared_ptr<WindowSystem> m_window_system;
    
    public:
        void startSystems(); ///< Start all systems
        void shutdownSystems(); ///< shutdown all System
    };

    extern RuntimeGlobalContext g_runtime_global_context;
}
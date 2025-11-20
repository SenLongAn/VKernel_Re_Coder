#pragma once

#include <memory>

/**
 * Manage all systems and managers
 */
namespace VKernel
{
    
    class WindowSystem;
    class RenderSystem;

    class RuntimeGlobalContext ///< Management system and manager
    {
        
    public:
        // systems and managers
        std::shared_ptr<WindowSystem> m_window_system;
        std::shared_ptr<RenderSystem> m_render_system;
    
    public:
        void startSystems(); ///< Start all systems
        void shutdownSystems(); ///< shutdown all System
    };

    extern RuntimeGlobalContext g_runtime_global_context;
}
#include "runtime/function/global/global_context.h"

#include "runtime/function/render/window_system.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/debugdraw/debug_draw_manager.h"

#include "runtime/function/framework/world/world_manager.h"

namespace VKernel
{
    RuntimeGlobalContext g_runtime_global_context; ///< instance

    void RuntimeGlobalContext::startSystems()
    {
        m_world_manager = std::make_shared<WorldManager>();
        m_world_manager->initialize();

        m_window_system = std::make_shared<WindowSystem>();
        WindowCreateInfo window_create_info;
        m_window_system->initialize(window_create_info);

        m_render_system = std::make_shared<RenderSystem>();
        m_render_system->initialize(m_window_system);
        
        m_debugdraw_manager = std::make_shared<DebugDrawManager>();
        m_debugdraw_manager->initialize();
    }

    void RuntimeGlobalContext::shutdownSystems()
    {
        m_debugdraw_manager.reset();
        
        m_render_system->clear();
        m_render_system.reset();

        m_window_system.reset();

        m_world_manager->clear();
        m_world_manager.reset();
    }
}
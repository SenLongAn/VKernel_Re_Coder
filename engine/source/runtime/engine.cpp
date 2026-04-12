#include "runtime/engine.h"

#include "runtime/core/base/macro.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

#include "runtime/function/input/input_system.h"

#include "runtime/function/framework/world/world_manager.h"

#include "runtime/core/meta/reflection/reflection_register.h"

#include <iostream>

namespace VKernel
{
    bool g_is_editor_mode{false};
    bool g_is_update_mode{true};
    bool g_is_full_screen_mode{false};
    std::unordered_set<std::string> g_editor_tick_component_types{}; ///< Component that only runs in editor mode

    void VKernelEngine::startEngine(const std::string &config_file_path)
    {
        Reflection::TypeMetaRegister::metaRegister(); ///< meta Register

        g_runtime_global_context.startSystems(config_file_path);

        LOG_INFO("engine start");
    }

    void VKernelEngine::run() {}

    bool VKernelEngine::tickOneFrame(float delta_time)
    {
        // tick
        logicalTick(delta_time);
        g_runtime_global_context.m_render_system
            ->swapLogicRenderData(); ///< exchange data between logic and render contexts
        rendererTick(delta_time);

        // check window
        g_runtime_global_context.m_window_system->pollEvents();
        const bool should_window_close = g_runtime_global_context.m_window_system->shouldClose();
        return !should_window_close;
    }

    void VKernelEngine::shutdownEngine() { g_runtime_global_context.shutdownSystems(); }

    void VKernelEngine::logicalTick(float delta_time)
    {
        g_runtime_global_context.m_world_manager->tick(delta_time);
        g_runtime_global_context.m_input_system->tick();
    }

    bool VKernelEngine::rendererTick(float delta_time)
    {
        g_runtime_global_context.m_render_system->tick(delta_time);
        return true;
    }

    float VKernelEngine::calculateDeltaTime()
    {
        float delta_time;
        {
            using namespace std::chrono;

            steady_clock::time_point tick_time_point = steady_clock::now(); ///< current time
            duration<float> time_span = duration_cast<duration<float>>(
                tick_time_point - m_last_tick_time_point); ///< Calculate the time difference
            delta_time = time_span.count();

            m_last_tick_time_point = tick_time_point;
        }
        return delta_time;
    }
} // namespace VKernel
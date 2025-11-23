#include "runtime/function/render/debugdraw/debug_draw_manager.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"

namespace VKernel
{
    void DebugDrawManager::initialize()
    {
        m_vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();
    }
}
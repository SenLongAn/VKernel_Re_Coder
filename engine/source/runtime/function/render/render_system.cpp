#include "runtime/function/render/render_system.h"

#include "runtime/function/render/vulkan_interface/vulkan_api.h"

namespace VKernel
{

    RenderSystem::~RenderSystem()
    {
        clear();
    }

    void RenderSystem::initialize(std::shared_ptr<WindowSystem> window_system)
    {
        m_vulkan_api = std::make_shared<VulkanAPI>();
        m_vulkan_api->initialize(window_system);
    }

    void RenderSystem::clear()
    {
        if (m_vulkan_api)
        {
            m_vulkan_api->clear();
        }
        m_vulkan_api.reset();
    }
}
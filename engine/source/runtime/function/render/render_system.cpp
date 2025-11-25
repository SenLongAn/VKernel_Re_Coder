#include "runtime/function/render/render_system.h"

#include "runtime/function/render/vulkan_interface/vulkan_api.h"
#include "runtime/function/render/render_pipeline.h"

namespace VKernel
{

    RenderSystem::~RenderSystem()
    {
        clear();
    }

    void RenderSystem::initialize(std::shared_ptr<WindowSystem> window_system)
    {
        // init
        m_vulkan_api = std::make_shared<VulkanAPI>();
        m_vulkan_api->initialize(window_system);

        m_render_pipeline = std::make_shared<RenderPipeline>();
        m_render_pipeline->m_vulkan_api = m_vulkan_api;
        m_render_pipeline->initialize();
    }

    void RenderSystem::tick(float delta_time)
    {
        // prepare render command context, Update m_current_command_buffer
        m_vulkan_api->prepareContext();

        m_render_pipeline->forwardRender(m_vulkan_api);
    }

    void RenderSystem::clear()
    {
        if (m_vulkan_api)
        {
            m_vulkan_api->clear();
        }
        m_vulkan_api.reset();
    }

    std::shared_ptr<VulkanAPI> RenderSystem::getVulkanAPI() const { return m_vulkan_api; }
}
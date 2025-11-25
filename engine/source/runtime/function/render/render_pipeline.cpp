#include "runtime/function/render/render_pipeline.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/debugdraw/debug_draw_manager.h"

namespace VKernel
{
    void RenderPipeline::forwardRender(std::shared_ptr<VulkanAPI> vulkan_api)
    {
        // wait fence
        vkWaitForFences(vulkan_api->getLogicDevice(), 1, &vulkan_api->getFenceList()[vulkan_api->getCurrentFrameIndex()], VK_TRUE, UINT64_MAX);
        vkResetFences(vulkan_api->getLogicDevice(), 1,  &vulkan_api->getFenceList()[vulkan_api->getCurrentFrameIndex()]);

        // reset command buffer
        vkResetCommandBuffer(vulkan_api->getCurrentCommandBuffer(), 0);
        
        // acquire image and begin command buffer
        bool recreate_swapchain = vulkan_api->prepareBeforePass();
        if (recreate_swapchain)
        {
            return;
        }

        // begin render
        g_runtime_global_context.m_debugdraw_manager->draw(vulkan_api->getCurrentSwapchainImageIndex());

        // end command buffer, submit and present
        vulkan_api->submitRendering();
    }
}
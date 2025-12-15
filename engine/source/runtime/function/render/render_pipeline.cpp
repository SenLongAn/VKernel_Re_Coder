#include "runtime/function/render/render_pipeline.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/debugdraw/debug_draw_manager.h"
#include "runtime/function/render/passes/main_camera_pass.h"

namespace VKernel
{

    void RenderPipeline::initialize(RenderPipelineInitInfo init_info)
    {
        // init point
        m_main_camera_pass = std::make_shared<MainCameraPass>();

        // init info
        RenderPassCommonInfo pass_common_info;
        pass_common_info.vulkan_api      = m_vulkan_api;
        pass_common_info.render_resource = init_info.render_resource;

        m_main_camera_pass->setCommonInfo(pass_common_info);

        // init
        m_main_camera_pass->initialize();
    }

    void RenderPipeline::forwardRender(std::shared_ptr<VulkanAPI>          vulkan_api,
                                       std::shared_ptr<RenderResourceBase> render_resource)
    {
        // reset ring buffer offset
        RenderResource* vulkan_resource = static_cast<RenderResource*>(render_resource.get());

        vulkan_resource->resetRingBufferOffset(vulkan_api->getCurrentFrameIndex());

        // wait fence
        vkWaitForFences(vulkan_api->getLogicDevice(),
                        1,
                        &vulkan_api->getFenceList()[vulkan_api->getCurrentFrameIndex()],
                        VK_TRUE,
                        UINT64_MAX);

        // reset command buffer
        vkResetCommandBuffer(vulkan_api->getCurrentCommandBuffer(), 0);

        // acquire image and begin command buffer
        bool recreate_swapchain =
            vulkan_api->prepareBeforePass(std::bind(&RenderPipeline::passUpdateAfterRecreateSwapchain, this));
        if (recreate_swapchain)
        {
            return;
        }

        // begin render
        static_cast<MainCameraPass*>(m_main_camera_pass.get())
            ->drawForward(vulkan_api->getCurrentSwapchainImageIndex());

        g_runtime_global_context.m_debugdraw_manager->draw(vulkan_api->getCurrentSwapchainImageIndex());

        // end command buffer, submit and present
        vulkan_api->submitRendering(std::bind(&RenderPipeline::passUpdateAfterRecreateSwapchain, this));
    }

    void RenderPipeline::passUpdateAfterRecreateSwapchain()
    {
        MainCameraPass& main_camera_pass = *(static_cast<MainCameraPass*>(m_main_camera_pass.get()));
        main_camera_pass.updateAfterFramebufferRecreate();
        g_runtime_global_context.m_debugdraw_manager->updateAfterRecreateSwapchain();
    }
} // namespace VKernel
#include "runtime/function/render/render_pipeline.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/debugdraw/debug_draw_manager.h"
#include "runtime/function/render/passes/color_grading_pass.h"
#include "runtime/function/render/passes/combine_ui_pass.h"
#include "runtime/function/render/passes/directional_light_pass.h"
#include "runtime/function/render/passes/main_camera_pass.h"
#include "runtime/function/render/passes/pick_pass.h"
#include "runtime/function/render/passes/point_light_pass.h"
#include "runtime/function/render/passes/tone_mapping_pass.h"
#include "runtime/function/render/passes/ui_pass.h"
#include "runtime/function/ui/window_ui.h"

namespace VKernel
{

    void RenderPipeline::initialize(RenderPipelineInitInfo init_info)
    {
        // init point
        m_point_light_shadow_pass = std::make_shared<PointLightShadowPass>();
        m_directional_light_pass  = std::make_shared<DirectionalLightShadowPass>();
        m_main_camera_pass        = std::make_shared<MainCameraPass>();
        m_color_grading_pass      = std::make_shared<ColorGradingPass>();
        m_tone_mapping_pass       = std::make_shared<ToneMappingPass>();
        m_ui_pass                 = std::make_shared<UIPass>();
        m_combine_ui_pass         = std::make_shared<CombineUIPass>();
        m_fxaa_pass               = std::make_shared<FXAAPass>();
        m_pick_pass               = std::make_shared<PickPass>();
        m_debugdraw_manager       = std::make_shared<DebugDrawManager>();

        // init info
        RenderPassCommonInfo pass_common_info;
        pass_common_info.vulkan_api      = m_vulkan_api;
        pass_common_info.render_resource = init_info.render_resource;

        m_point_light_shadow_pass->setCommonInfo(pass_common_info);
        m_directional_light_pass->setCommonInfo(pass_common_info);
        m_main_camera_pass->setCommonInfo(pass_common_info);
        m_color_grading_pass->setCommonInfo(pass_common_info);
        m_tone_mapping_pass->setCommonInfo(pass_common_info);
        m_ui_pass->setCommonInfo(pass_common_info);
        m_combine_ui_pass->setCommonInfo(pass_common_info);
        m_fxaa_pass->setCommonInfo(pass_common_info);
        m_pick_pass->setCommonInfo(pass_common_info);

        // init
        m_point_light_shadow_pass->initialize(nullptr);
        m_directional_light_pass->initialize(nullptr);

        std::shared_ptr<MainCameraPass> main_camera_pass = std::static_pointer_cast<MainCameraPass>(m_main_camera_pass);

        main_camera_pass->m_point_light_shadow_color_image_view =
            std::static_pointer_cast<RenderPass>(m_point_light_shadow_pass)->m_framebuffer.attachments[0].view;
        main_camera_pass->m_directional_light_shadow_color_image_view =
            std::static_pointer_cast<RenderPass>(m_directional_light_pass)->m_framebuffer.attachments[0].view;

        std::shared_ptr<RenderPass> _main_camera_pass = std::static_pointer_cast<RenderPass>(m_main_camera_pass);

        MainCameraPassInitInfo main_camera_init_info;
        m_main_camera_pass->initialize(&main_camera_init_info);

        m_point_light_shadow_pass->postInitialize();
        m_directional_light_pass->postInitialize();

        ColorGradingPassInitInfo color_grading_init_info;
        color_grading_init_info.render_pass = _main_camera_pass->getRenderPass();
        color_grading_init_info.input_attachment =
            _main_camera_pass->getFramebufferImageViews()[_main_camera_pass_backup_buffer_odd];
        m_color_grading_pass->initialize(&color_grading_init_info);

        ToneMappingPassInitInfo tone_mapping_init_info;
        tone_mapping_init_info.render_pass = _main_camera_pass->getRenderPass();
        tone_mapping_init_info.input_attachment =
            _main_camera_pass->getFramebufferImageViews()[_main_camera_pass_backup_buffer_even];
        m_tone_mapping_pass->initialize(&tone_mapping_init_info);

        UIPassInitInfo ui_init_info;
        ui_init_info.render_pass = _main_camera_pass->getRenderPass(); ///< get RenderPass from main camera pass
        m_ui_pass->initialize(&ui_init_info);
        m_ui_pass->initializeUIRenderBackend();

        VKernel::WindowUIInitInfo window_ui_init_info = {VKernel::g_runtime_global_context.m_window_system,
                                                         VKernel::g_runtime_global_context.m_render_system};
        g_runtime_global_context.m_window_ui_manager->registerUIs();
        g_runtime_global_context.m_window_ui_manager->initUIs(window_ui_init_info);

        CombineUIPassInitInfo combine_ui_init_info;
        combine_ui_init_info.render_pass = _main_camera_pass->getRenderPass();
        combine_ui_init_info.scene_input_attachment =
            _main_camera_pass->getFramebufferImageViews()[_main_camera_pass_backup_buffer_odd]; ///< scene
        combine_ui_init_info.ui_input_attachment =
            _main_camera_pass->getFramebufferImageViews()[_main_camera_pass_backup_buffer_even]; ///< editor ui
        m_combine_ui_pass->initialize(&combine_ui_init_info);

        PickPassInitInfo pick_init_info;
        m_pick_pass->initialize(&pick_init_info);

        FXAAPassInitInfo fxaa_init_info;
        fxaa_init_info.render_pass = _main_camera_pass->getRenderPass();
        fxaa_init_info.input_attachment =
            _main_camera_pass->getFramebufferImageViews()[_main_camera_pass_backup_buffer_even];
        m_fxaa_pass->initialize(&fxaa_init_info);

        DebugPassInitInfo debug_init_info;
        debug_init_info.render_pass = _main_camera_pass->getRenderPass();
        m_debugdraw_manager->initialize(&debug_init_info);
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
        static_cast<DirectionalLightShadowPass*>(m_directional_light_pass.get())
            ->draw(); ///< directional light pass shadow
        static_cast<PointLightShadowPass*>(m_point_light_shadow_pass.get())->draw();

        ColorGradingPass& color_grading_pass = *(static_cast<ColorGradingPass*>(m_color_grading_pass.get()));
        ToneMappingPass&  tone_mapping_pass  = *(static_cast<ToneMappingPass*>(m_tone_mapping_pass.get()));
        FXAAPass&         fxaa_pass          = *(static_cast<FXAAPass*>(m_fxaa_pass.get()));
        UIPass&           ui_pass            = *(static_cast<UIPass*>(m_ui_pass.get()));
        CombineUIPass&    combine_ui_pass    = *(static_cast<CombineUIPass*>(m_combine_ui_pass.get()));

        static_cast<MainCameraPass*>(m_main_camera_pass.get())
            ->drawForward(color_grading_pass,
                          tone_mapping_pass,
                          fxaa_pass,
                          ui_pass,
                          combine_ui_pass,
                          vulkan_api->getCurrentSwapchainImageIndex()); ///< main camera

        // end command buffer, submit and present
        vulkan_api->submitRendering(std::bind(&RenderPipeline::passUpdateAfterRecreateSwapchain, this));
    }

    void RenderPipeline::deferredRender(std::shared_ptr<VulkanAPI>          vulkan_api,
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
        static_cast<DirectionalLightShadowPass*>(m_directional_light_pass.get())
            ->draw(); ///< directional light pass shadow
        static_cast<PointLightShadowPass*>(m_point_light_shadow_pass.get())->draw();

        ColorGradingPass& color_grading_pass = *(static_cast<ColorGradingPass*>(m_color_grading_pass.get()));
        ToneMappingPass&  tone_mapping_pass  = *(static_cast<ToneMappingPass*>(m_tone_mapping_pass.get()));
        FXAAPass&         fxaa_pass          = *(static_cast<FXAAPass*>(m_fxaa_pass.get()));
        UIPass&           ui_pass            = *(static_cast<UIPass*>(m_ui_pass.get()));
        CombineUIPass&    combine_ui_pass    = *(static_cast<CombineUIPass*>(m_combine_ui_pass.get()));

        static_cast<MainCameraPass*>(m_main_camera_pass.get())
            ->draw(color_grading_pass,
                   tone_mapping_pass,
                   fxaa_pass,
                   ui_pass,
                   combine_ui_pass,
                   vulkan_api->getCurrentSwapchainImageIndex()); ///< main camera

        // end command buffer, submit and present
        vulkan_api->submitRendering(std::bind(&RenderPipeline::passUpdateAfterRecreateSwapchain, this));
    }

    void RenderPipeline::passUpdateAfterRecreateSwapchain()
    {
        MainCameraPass&   main_camera_pass   = *(static_cast<MainCameraPass*>(m_main_camera_pass.get()));
        ColorGradingPass& color_grading_pass = *(static_cast<ColorGradingPass*>(m_color_grading_pass.get()));
        ToneMappingPass&  tone_mapping_pass  = *(static_cast<ToneMappingPass*>(m_tone_mapping_pass.get()));
        FXAAPass&         fxaa_pass          = *(static_cast<FXAAPass*>(m_fxaa_pass.get()));
        CombineUIPass&    combine_ui_pass    = *(static_cast<CombineUIPass*>(m_combine_ui_pass.get()));
        PickPass&         pick_pass          = *(static_cast<PickPass*>(m_pick_pass.get()));

        main_camera_pass.updateAfterFramebufferRecreate();
        tone_mapping_pass.updateAfterFramebufferRecreate(
            main_camera_pass.getFramebufferImageViews()[_main_camera_pass_backup_buffer_even]);
        color_grading_pass.updateAfterFramebufferRecreate(
            main_camera_pass.getFramebufferImageViews()[_main_camera_pass_backup_buffer_odd]);
        fxaa_pass.updateAfterFramebufferRecreate(
            main_camera_pass.getFramebufferImageViews()[_main_camera_pass_backup_buffer_even]);
        combine_ui_pass.updateAfterFramebufferRecreate(
            main_camera_pass.getFramebufferImageViews()[_main_camera_pass_backup_buffer_odd],
            main_camera_pass.getFramebufferImageViews()[_main_camera_pass_backup_buffer_even]);
        pick_pass.recreateFramebuffer();

        // m_debugdraw_manager->updateAfterRecreateSwapchain();
    }

    uint32_t RenderPipeline::getGuidOfPickedMesh(const Vector2& picked_uv)
    {
        PickPass& pick_pass = *(static_cast<PickPass*>(m_pick_pass.get()));
        return pick_pass.pick(picked_uv);
    }

    void RenderPipeline::setAxisVisibleState(bool state)
    {
        MainCameraPass& main_camera_pass = *(static_cast<MainCameraPass*>(m_main_camera_pass.get()));
        main_camera_pass.m_is_show_axis  = state; ///< set main camera memeber
    }

    void RenderPipeline::setSelectedAxis(size_t selected_axis)
    {
        MainCameraPass& main_camera_pass = *(static_cast<MainCameraPass*>(m_main_camera_pass.get()));
        main_camera_pass.m_selected_axis = selected_axis;
    }
} // namespace VKernel

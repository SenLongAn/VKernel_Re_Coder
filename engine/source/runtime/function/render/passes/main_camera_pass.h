#pragma once

#include "runtime/function/render/passes/color_grading_pass.h"
#include "runtime/function/render/passes/combine_ui_pass.h"
#include "runtime/function/render/passes/fxaa_pass.h"
#include "runtime/function/render/passes/tone_mapping_pass.h"
#include "runtime/function/render/passes/ui_pass.h"
#include "runtime/function/render/render_pass.h"


/**
 * scene render core
 */
namespace VKernel
{

    class RenderResourceBase;

    struct MainCameraPassInitInfo : RenderPassInitInfo
    {};

    class MainCameraPass : public RenderPass
    {
    public:
        enum LayoutType : uint8_t // descriptor layout
        {
            _mesh_global = 0,   ///< MVP
            _mesh_per_material, ///< material texture
            _skybox,            ///< skybox
            _axis,              ///< Gizmo axis
            _deferred_lighting, ///< deferred
            _layout_type_count
        };

        enum RenderPipeLineType : uint8_t ///< pipline type
        {
            _render_pipeline_type_mesh_gbuffer = 0,  ///< deferred GBuffer
            _render_pipeline_type_deferred_lighting, ///< deferred lighting
            _render_pipeline_type_mesh_lighting,     ///< forward render
            _render_pipeline_type_skybox,            ///< skybox
            _render_pipeline_type_axis,              ///< Gizmo axis
            _render_pipeline_type_count
        };

    public:
        // axis
        bool   m_is_show_axis {false};
        size_t m_selected_axis {3};

    public:
        void initialize(const RenderPassInitInfo* init_info) override final; ///< init

        void preparePassData(std::shared_ptr<RenderResourceBase> render_resource) override final; ///< get other class

        void draw(ColorGradingPass& color_grading_pass,
                  ToneMappingPass&  tone_mapping_pass,
                  FXAAPass&         fxaa_pass,
                  UIPass&           ui_pass,
                  CombineUIPass&    combine_ui_pass,
                  uint32_t          current_swapchain_image_index); ///< deferred

        void drawForward(ColorGradingPass& color_grading_pass,
                         ToneMappingPass&  tone_mapping_pass,
                         FXAAPass&         fxaa_pass,
                         UIPass&           ui_pass,
                         CombineUIPass&    combine_ui_pass,
                         uint32_t          current_swapchain_image_index); ///< forward

        void updateAfterFramebufferRecreate(); ///< recreate framebuffer

        // shadow image view
        VkImageView m_point_light_shadow_color_image_view;
        VkImageView m_directional_light_shadow_color_image_view;

    private:
        std::vector<VkFramebuffer> m_swapchain_framebuffers; ///< frame buffer

        MeshPerframeStorageBufferObject m_mesh_perframe_storage_buffer_object;

        // axis
        AxisStorageBufferObject m_axis_storage_buffer_object;

    private:
        // init
        void setupAttachments(); ///< attachment image and iamge view
        void setupRenderPass();  ///< renderpass
        void setupDescriptorSetLayout();
        void setupPipelines(); ///< pipline
        void setupDescriptorSet();
        void setupFramebufferDescriptorSet();
        void setupSwapchainFramebuffers(); ///< Framebuffer

        void setupModelGlobalDescriptorSet(); ///< bind buffer and set
        void setupSkyboxDescriptorSet();
        void setupAxisDescriptorSet();
        void setupGbufferLightingDescriptorSet();

        // draw
        void drawMeshGbuffer();
        void drawDeferredLighting();
        void drawMeshLighting(); ///< forward render scene
        void drawSkybox();
        void drawAxis();
    };
} // namespace VKernel
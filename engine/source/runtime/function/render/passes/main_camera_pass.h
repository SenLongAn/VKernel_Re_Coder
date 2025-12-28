#pragma once

#include "runtime/function/render/passes/combine_ui_pass.h"
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
            _layout_type_count
        };

        enum RenderPipeLineType : uint8_t ///< pipline type
        {
            _render_pipeline_type_mesh_lighting = 0, ///< forward render
            _render_pipeline_type_skybox,            // skybox
            _render_pipeline_type_count
        };

    public:
        void initialize(const RenderPassInitInfo* init_info) override final; ///< init

        void preparePassData(std::shared_ptr<RenderResourceBase> render_resource) override final; ///< get other class

        void drawForward(UIPass&        ui_pass,
                         CombineUIPass& combine_ui_pass,
                         uint32_t       current_swapchain_image_index); ///< draw: all subpass

        void updateAfterFramebufferRecreate(); ///< recreate framebuffer

    private:
        std::vector<VkFramebuffer> m_swapchain_framebuffers; ///< frame buffer

        MeshPerframeStorageBufferObject m_mesh_perframe_storage_buffer_object;

    private:
        // init
        void setupAttachments(); ///< attachment image and iamge view
        void setupRenderPass();  ///< renderpass
        void setupDescriptorSetLayout();
        void setupPipelines(); ///< pipline
        void setupDescriptorSet();
        void setupSwapchainFramebuffers(); ///< Framebuffer

        void setupModelGlobalDescriptorSet(); ///< bind buffer and set
        void setupSkyboxDescriptorSet();

        // draw
        void drawMeshLighting(); ///< forward render scene
        void drawSkybox();
    };
} // namespace VKernel
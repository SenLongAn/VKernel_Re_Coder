#pragma once

#include "runtime/function/render/render_pass.h"

/**
 * render core
 */
namespace VKernel
{

    class RenderResourceBase;

    enum RenderPipeLineType : uint8_t ///< pipline type
    {
        _render_pipeline_type_mesh_lighting = 0, ///< forward render
        _render_pipeline_type_count
    };

    class MainCameraPass : public RenderPass
    {

        enum LayoutType : uint8_t
        {
            _mesh_global = 0,
            _layout_type_count
        };

    public:
        void initialize() override final; ///< init

        void preparePassData(std::shared_ptr<RenderResourceBase> render_resource) override final;

        void drawForward(uint32_t current_swapchain_image_index); ///< draw

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

        // draw
        void drawMeshLighting(); ///< forward render
    };
} // namespace VKernel
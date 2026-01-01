#pragma once

#include "runtime/function/render/render_pass.h"

/**
 * Directional Light Shadow
 */
namespace VKernel
{
    class RenderResourceBase;

    class DirectionalLightShadowPass : public RenderPass
    {
    public:
        void initialize(const RenderPassInitInfo* init_info) override final; ///< init
        void postInitialize() override final;                                ///< init

        void preparePassData(std::shared_ptr<RenderResourceBase> render_resource) override final; ///< get other class

        void draw() override final; ///< draw

    private:
        // init
        void setupAttachments();
        void setupRenderPass();
        void setupFramebuffer();
        void setupDescriptorSetLayout();

        void setupPipelines();
        void setupDescriptorSet();

        // draw
        void drawModel();

    private:
        MeshDirectionalLightShadowPerframeStorageBufferObject
            m_mesh_directional_light_shadow_perframe_storage_buffer_object; ///< light PV
    };
} // namespace VKernel

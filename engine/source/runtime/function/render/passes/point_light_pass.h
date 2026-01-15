#pragma once

#include "runtime/function/render/render_pass.h"

namespace VKernel
{
    class RenderResourceBase;

    class PointLightShadowPass : public RenderPass
    {
    public:
        // init
        void initialize(const RenderPassInitInfo* init_info) override final;
        void postInitialize() override final;

        // set
        void preparePassData(std::shared_ptr<RenderResourceBase> render_resource) override final;

        // draw
        void draw() override final;

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
        MeshPointLightShadowPerframeStorageBufferObject m_mesh_point_light_shadow_perframe_storage_buffer_object;
    };
} // namespace VKernel

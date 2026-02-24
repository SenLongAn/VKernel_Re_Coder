#pragma once

#include "runtime/core/math/vector2.h"
#include "runtime/function/render/render_pass.h"

namespace VKernel
{
    class RenderResourceBase;

    struct PickPassInitInfo : RenderPassInitInfo
    {};

    class PickPass : public RenderPass
    {
    public:
        void initialize(const RenderPassInitInfo* init_info) override final;                      ///< init
        void postInitialize() override final;                                                     ///< delay init
        void preparePassData(std::shared_ptr<RenderResourceBase> render_resource) override final; ///< load data

        void                         draw() override final;          /// draw
        std::pair<uint32_t, Vector4> pick(const Vector2& picked_uv); ///< draw

        void recreateFramebuffer(); ///< reset framebuffer

        MeshInefficientPickPerframeStorageBufferObject _mesh_inefficient_pick_perframe_storage_buffer_object;

    private:
        // init
        void setupAttachments();
        void setupRenderPass();
        void setupFramebuffer();
        void setupDescriptorSetLayout();
        void setupPipelines();
        void setupDescriptorSet();

    private:
        VkDescriptorSetLayout _per_mesh_layout = VK_NULL_HANDLE;
    };
} // namespace VKernel

#include "runtime/function/render/render_pass_base.h"

namespace VKernel
{
    void RenderPassBase::postInitialize() {}

    void RenderPassBase::setCommonInfo(RenderPassCommonInfo common_info)
    {
        m_vulkan_api      = common_info.vulkan_api;
        m_render_resource = common_info.render_resource;
    }

    void RenderPassBase::preparePassData(std::shared_ptr<RenderResourceBase> render_resource) {}
    void RenderPassBase::initializeUIRenderBackend() {}
} // namespace VKernel
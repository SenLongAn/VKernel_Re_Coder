#include "runtime/function/render/render_pass.h"

#include "render_pass.h"
#include "runtime/function/render/render_resource.h"

VKernel::VisiableNodes VKernel::RenderPass::m_visiable_nodes;

namespace VKernel
{
    void RenderPass::initialize(const RenderPassInitInfo* init_info)
    {
        m_global_render_resource =
            &(std::static_pointer_cast<RenderResource>(m_render_resource)->m_global_render_resource);
    }

    VkRenderPass RenderPass::getRenderPass() const { return m_framebuffer.render_pass; }

    void RenderPass::draw() {}

    std::vector<VkImageView> RenderPass::getFramebufferImageViews() const
    {
        std::vector<VkImageView> image_views;
        for (auto& attach : m_framebuffer.attachments)
        {
            image_views.push_back(attach.view);
        }
        return image_views;
    }
} // namespace VKernel

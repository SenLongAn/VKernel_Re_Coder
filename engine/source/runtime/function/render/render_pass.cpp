#include "runtime/function/render/render_pass.h"

#include "runtime/function/render/render_resource.h"

VKernel::VisiableNodes VKernel::RenderPass::m_visiable_nodes;

namespace VKernel
{
    void RenderPass::initialize()
    {
        m_global_render_resource =
            &(std::static_pointer_cast<RenderResource>(m_render_resource)->m_global_render_resource);
    }

} // namespace VKernel

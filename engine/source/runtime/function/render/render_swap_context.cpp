#include "runtime/function/render/render_swap_context.h"

namespace VKernel
{
    RenderSwapData &RenderSwapContext::getLogicSwapData() { return m_swap_data[m_logic_swap_data_index]; }

    RenderSwapData &RenderSwapContext::getRenderSwapData() { return m_swap_data[m_render_swap_data_index]; }

    void RenderSwapContext::resetCameraSwapData() { m_swap_data[m_render_swap_data_index].m_camera_swap_data.reset(); }

    void RenderSwapContext::swapLogicRenderData()
    {
        if (isReadyToSwap())
        {
            swap();
        }
    }

    bool RenderSwapContext::isReadyToSwap() const
    {
        return !(m_swap_data[m_render_swap_data_index].m_camera_swap_data.has_value());
    }

    void RenderSwapContext::swap()
    {
        resetCameraSwapData();
        std::swap(m_logic_swap_data_index, m_render_swap_data_index);
    }
}
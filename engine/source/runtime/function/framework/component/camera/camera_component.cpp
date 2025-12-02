#include "runtime/function/framework/component/camera/camera_component.h"

#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"

namespace VKernel
{
    void CameraComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        m_parent_object = parent_object;

        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData camera_swap_data;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }
    
    void CameraComponent::tick(float delta_time)
    {
        m_position = {-5.0f, 0.0f, -1.0f};

        Matrix4x4 desired_mat = Math::makeLookAtMatrix(m_position, {0.0f, 0.0f, 0.0f} , m_up);
        
        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData camera_swap_data;
        camera_swap_data.m_fov_x = 45.0f;
        camera_swap_data.m_view_matrix = desired_mat;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }
}
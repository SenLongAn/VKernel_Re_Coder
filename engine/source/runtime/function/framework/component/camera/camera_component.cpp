#include "runtime/function/framework/component/camera/camera_component.h"

#include "runtime/function/global/global_context.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/render_camera.h"

#include "runtime/function/input/input_system.h"


#include <iostream>
namespace VKernel
{
    void CameraComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        m_parent_object = parent_object;

        RenderSwapContext &swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData camera_swap_data;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }

    void CameraComponent::tick(float delta_time)
    {
        RenderSwapContext &swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData camera_swap_data;
        camera_swap_data.m_fov_x = 50.0f;
        // camera_swap_data.m_view_matrix = desired_mat;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }
}
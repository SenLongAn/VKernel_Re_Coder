#include "runtime/function/framework/component/camera/camera_component.h"

#include "runtime/function/global/global_context.h"

#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_system.h"

#include "runtime/function/input/input_system.h"

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
        // get user input
        unsigned int command = g_runtime_global_context.m_input_system->getGameCommand();
        if (command >= (unsigned int)GameCommand::invalid)
            return;

        bool has_move_command = ((unsigned int)GameCommand::forward | (unsigned int)GameCommand::backward |
                                 (unsigned int)GameCommand::left | (unsigned int)GameCommand::right) &
                                command;
        if (has_move_command)
        {
            Vector3 move_direction = Vector3::ZERO;

            if ((unsigned int)GameCommand::forward & command)
            {
                move_direction += m_forward;
            }

            if ((unsigned int)GameCommand::backward & command)
            {
                move_direction -= m_forward;
            }

            if ((unsigned int)GameCommand::left & command)
            {
                move_direction += m_left;
            }

            if ((unsigned int)GameCommand::right & command)
            {
                move_direction -= m_left;
            }

            m_position += move_direction * 2.0f * delta_time;
        }

        // m_position = {0.0f, 0.0f, -1.0f};

        Matrix4x4 desired_mat = Math::makeLookAtMatrix(m_position, {0.0f, 0.0f, 0.0f}, m_up);

        RenderSwapContext &swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData camera_swap_data;
        camera_swap_data.m_fov_x = 45.0f;
        camera_swap_data.m_view_matrix = desired_mat;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }
}
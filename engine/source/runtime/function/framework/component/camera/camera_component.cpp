#include "runtime/function/framework/component/camera/camera_component.h"

#include "runtime/function/global/global_context.h"

#include "runtime/function/character/character.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_system.h"

#include "runtime/function/input/input_system.h"

#include <iostream>
namespace VKernel
{
    void CameraComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        // Set Associated Object
        m_parent_object = parent_object;

        //
        // RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        // CameraSwapData     camera_swap_data;
        // camera_swap_data.m_fov_x                           = 50.0f;
        // swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }

    void CameraComponent::tick(float delta_time)
    {
        // Check
        if (!m_parent_object.lock())
            return;
        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;
        if (current_character->getObjectID() != m_parent_object.lock()->getID())
            return;

        // update
        tickThirdPersonCamera(delta_time);

        //
        // RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        // CameraSwapData     camera_swap_data;
        // swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }

    void CameraComponent::tickThirdPersonCamera(float delta_time)
    {
        // get character
        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;

        // Calculate delta Q
        Quaternion q_yaw, q_pitch;
        q_yaw.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_yaw, Vector3::NEGATIVE_UNIT_Y);
        q_pitch.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_pitch, Vector3::UNIT_X);

        // update new pitch Q
        ThirdPersonCameraParameter* param = static_cast<ThirdPersonCameraParameter*>(m_camera_res.m_parameter);
        param->m_cursor_pitch             = q_pitch * param->m_cursor_pitch;

        // get
        const float vertical_offset   = param->m_vertical_offset;
        const float horizontal_offset = param->m_horizontal_offset;
        Vector3     offset            = Vector3(0, vertical_offset, horizontal_offset);

        // calculate camera new position
        Vector3 center_pos = current_character->getPosition() + Vector3::UNIT_Y * vertical_offset;
        m_position =
            current_character->getRotation() * param->m_cursor_pitch * offset + current_character->getPosition();

        // calculate camera new rotation
        m_forward = center_pos - m_position;
        m_up      = current_character->getRotation() * param->m_cursor_pitch * Vector3::NEGATIVE_UNIT_Y;
        m_left    = m_up.crossProduct(m_forward);

        // set charactor new rotation
        current_character->setRotation(q_yaw * current_character->getRotation());

        // set swap data
        Matrix4x4          desired_mat  = Math::makeLookAtMatrix(m_position, m_position + m_forward, m_up);
        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData     camera_swap_data;
        camera_swap_data.m_view_matrix                     = desired_mat;
        camera_swap_data.m_camera_type                     = RenderCameraType::Motor;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }
} // namespace VKernel
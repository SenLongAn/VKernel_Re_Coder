#include "runtime/function/framework/component/camera/camera_component.h"

#include "runtime/core/base/macro.h"

#include "runtime/function/character/character.h"
#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

#include <iostream>
namespace VKernel
{
    void CameraComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        // Set Associated Object
        m_parent_object = parent_object;

        // set camera mode
        const std::string& camera_type_name = m_camera_res.m_parameter.getTypeName();
        if (camera_type_name == "FirstPersonCameraParameter")
        {
            m_camera_mode = CameraMode::first_person;
        }
        else if (camera_type_name == "ThirdPersonCameraParameter")
        {
            m_camera_mode = CameraMode::third_person;
        }
        else
        {
            LOG_ERROR("invalid camera type");
        }
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
        switch (m_camera_mode)
        {
            case CameraMode::first_person:
                tickFirstPersonCamera(delta_time);
                break;
            case CameraMode::third_person:
                tickThirdPersonCamera(delta_time);
                break;
            default:
                break;
        }
    }

    void CameraComponent::tickFirstPersonCamera(float delta_time)
    {
        // get character
        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;

        // Calculate delta Q
        Quaternion q_yaw, q_pitch;
        q_yaw.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_yaw, Vector3::NEGATIVE_UNIT_Y);
        q_pitch.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_pitch, m_left);

        // calculate camera new position
        const float offset = static_cast<FirstPersonCameraParameter*>(m_camera_res.m_parameter)->m_vertical_offset;
        m_position         = current_character->getPosition() + offset * Vector3::UNIT_Y;

        // calculate camera new rotation
        m_forward = q_yaw * q_pitch * m_forward;
        m_left    = q_yaw * q_pitch * m_left;
        m_up      = m_forward.crossProduct(m_left);

        // set swap data
        Matrix4x4          desired_mat  = Math::makeLookAtMatrix(m_position, m_position + m_forward, m_up);
        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData     camera_swap_data;
        camera_swap_data.m_camera_type                     = RenderCameraType::Motor;
        camera_swap_data.m_view_matrix                     = desired_mat;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }

    void CameraComponent::tickThirdPersonCamera(float delta_time)
    {
        unsigned int command = g_runtime_global_context.m_input_system->getGameCommand();

        if (g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ||
            command < (unsigned int)GameCommand::invalid)
        {
            // get character
            std::shared_ptr<Level> current_level =
                g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
            std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
            if (current_character == nullptr)
                return;

            // Calculate cursor delta Q
            Quaternion q_yaw, q_pitch;
            q_yaw.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_yaw, Vector3::NEGATIVE_UNIT_Y);
            q_pitch.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_pitch,
                                  Vector3::NEGATIVE_UNIT_X);

            // update new pitch Q
            ThirdPersonCameraParameter* param = static_cast<ThirdPersonCameraParameter*>(m_camera_res.m_parameter);
            static Quaternion           last_pitch;
            static Quaternion           last_yaw;

            // get
            const float vertical_offset   = param->m_vertical_offset;
            const float horizontal_offset = param->m_horizontal_offset;
            Vector3     offset            = Vector3(0, vertical_offset, horizontal_offset);

            // set charactor new rotation

            if (g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) || isButtonRight)
            {
                // calculate camera new position
                m_position = current_character->getRotation() * param->m_cursor_pitch * offset +
                             current_character
                                 ->getPosition(); ///< camera position: yaw * pitch * offset vector + charactor position

                // calculate camera new rotation
                Vector3 center_pos =
                    current_character->getPosition() + Vector3::UNIT_Y * vertical_offset; ///< look at target
                m_forward = center_pos - m_position;
                m_up      = current_character->getRotation() * param->m_cursor_pitch *
                       Vector3::NEGATIVE_UNIT_Y; ///<  yaw * pitch * Y vector
                m_left = m_up.crossProduct(m_forward);

                last_pitch = param->m_cursor_pitch;
                last_yaw   = current_character->getRotation();

                isFirst = true;

                isButtonRight = true;

                if (g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
                {
                    // update new yaw and pitch
                    current_character->setRotation(q_yaw * current_character->getRotation());
                    param->m_cursor_pitch = q_pitch * param->m_cursor_pitch;
                }
                else
                {
                    isButtonRight = false;
                }
            }
            else
            {
                isFirst = false;
                // calculate camera new position
                m_position = last_yaw * last_pitch * offset + current_character->getPosition();
            }

            // set swap data
            Matrix4x4          desired_mat  = Math::makeLookAtMatrix(m_position, m_position + m_forward, m_up);
            RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
            CameraSwapData     camera_swap_data;
            camera_swap_data.m_view_matrix                     = desired_mat;
            camera_swap_data.m_camera_type                     = RenderCameraType::Motor;
            swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
        }
    }
} // namespace VKernel
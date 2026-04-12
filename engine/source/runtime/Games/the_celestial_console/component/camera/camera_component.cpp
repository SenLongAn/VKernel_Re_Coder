#include "runtime/Games/the_celestial_console/component/camera/camera_component.h"

#include "runtime/core/base/macro.h"

#include "runtime/Games/the_celestial_console/component/motor/motor_component.h"
#include "runtime/Games/the_celestial_console/control_cabin.h"

#include "runtime/core/math/vector3.h"
#include "runtime/core/math/vector4.h"
#include "runtime/function/character/character.h"
#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/render/debugdraw/debug_draw_manager.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

#include <iostream>

namespace Games
{
    void CameraComponent::postLoadResource(std::weak_ptr<VKernel::GObject> parent_object)
    {
        // Set Associated Object
        m_parent_object = parent_object;
    }

    void CameraComponent::tick(float delta_time)
    {
        // Check
        if (!m_parent_object.lock())
            return;
        std::shared_ptr<VKernel::Level> current_level =
            VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<VKernel::Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;
        if (current_character->getObjectID() != m_parent_object.lock()->getID())
            return;

        // update camera mode
        unsigned int command = VKernel::g_runtime_global_context.m_input_system->getGameCommand();
        if ((((unsigned int)VKernel::GameCommand::first_camera & command) > 0))
        {
            m_camera_mode = CameraMode::first_person;
        }
        else
        {
            m_camera_mode = CameraMode::third_person;
        }

        if (m_pre_camera_mode != m_camera_mode)
        {
            m_pre_camera_mode = m_camera_mode;
            isFirst = true;
        }

        // update camera positon and rotation
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

        // ray
        VKernel::Vector3 world_pos(VKernel::g_runtime_global_context.m_render_system->getPos().x,
                                   VKernel::g_runtime_global_context.m_render_system->getPos().y,
                                   VKernel::g_runtime_global_context.m_render_system->getPos().z);
        VKernel::g_runtime_global_context.m_render_system->getRenderPipline()
            ->getDebugManager()
            ->getDebugDrawGroup()
            ->addLine(VKernel::Vector4(0.00f, 1.0f, 1.0f, 1.00f),
                      std::static_pointer_cast<Games::ControlCabin>(current_character)->getPosition(),
                      world_pos,
                      VKernel::Transform(
                          VKernel::Vector3::ZERO, VKernel::Quaternion::IDENTITY, VKernel::Vector3::UNIT_SCALE));
    }

    std::pair<VKernel::Radian, VKernel::Radian> CameraComponent::calculateCursorDeltaAngles(double delta_x,
                                                                                            double delta_y)
    {
        // get window_size
        std::array<int, 2> window_size = VKernel::g_runtime_global_context.m_window_system->getWindowSize();

        if (window_size[0] < 1 || window_size[1] < 1)
        {
            return {VKernel::Radian(0), VKernel::Radian(0)};
        }

        // get camera fov
        std::shared_ptr<VKernel::RenderCamera> render_camera =
            VKernel::g_runtime_global_context.m_render_system->getRenderCamera();
        const VKernel::Vector2 &fov = render_camera->getFOV();

        // degrees To Radians
        VKernel::Radian cursor_delta_x(VKernel::Math::degreesToRadians(delta_x));
        VKernel::Radian cursor_delta_y(VKernel::Math::degreesToRadians(delta_y));

        // Calculate Radians
        VKernel::Radian m_cursor_delta_yaw = (cursor_delta_x / (float)window_size[0]) * fov.x;
        VKernel::Radian m_cursor_delta_pitch = -(cursor_delta_y / (float)window_size[1]) * fov.y;
        return {m_cursor_delta_yaw, m_cursor_delta_pitch};
    }

    void CameraComponent::tickFirstPersonCamera(float delta_time)
    {
        unsigned int command = VKernel::g_runtime_global_context.m_input_system->getGameCommand();
        MotorComponent *motor_component = (VKernel::g_runtime_global_context.m_character_Manager->getCurrentCharacter())
                                              ->getObject()
                                              .lock()
                                              ->tryGetComponent(MotorComponent, "MotorComponent");

        if (VKernel::g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ||
            command < (unsigned int)VKernel::GameCommand::invalid || isFirst)
        {

            // get character
            std::shared_ptr<VKernel::Level> current_level =
                VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
            std::shared_ptr<VKernel::Character> current_character = current_level->getCurrentActiveCharacter().lock();
            if (current_character == nullptr)
                return;

            // Calculate delta Q
            VKernel::Quaternion q_yaw, q_pitch;

            auto x = calculateCursorDeltaAngles(VKernel::g_runtime_global_context.m_input_system->m_cursor_delta_x,
                                                VKernel::g_runtime_global_context.m_input_system->m_cursor_delta_y);
            q_yaw.fromAngleAxis(x.first, VKernel::Vector3::NEGATIVE_UNIT_Y);
            q_pitch.fromAngleAxis(x.second, m_left);

            if (VKernel::g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
            {
                // calculate camera new position
                VKernel::FirstPersonCameraParameter *param = &(m_camera_res.m_first_camera);
                const float offset = param->m_vertical_offset;
                m_position = std::static_pointer_cast<Games::ControlCabin>(current_character)->getPosition() +
                             offset * VKernel::Vector3::UNIT_Y;

                // calculate camera new rotation
                m_forward = q_yaw * q_pitch * m_forward;
                m_left = q_yaw * q_pitch * m_left;
                m_up = m_forward.crossProduct(m_left);
            }
            else
            {
                // calculate camera new position
                VKernel::FirstPersonCameraParameter *param = &(m_camera_res.m_first_camera);
                const float offset = param->m_vertical_offset;
                m_position = std::static_pointer_cast<Games::ControlCabin>(current_character)->getPosition() +
                             offset * VKernel::Vector3::UNIT_Y;

                isFirst = false;
            }

            // set swap data
            VKernel::Matrix4x4 desired_mat = VKernel::Math::makeLookAtMatrix(m_position, m_position + m_forward, m_up);
            VKernel::RenderSwapContext &swap_context =
                VKernel::g_runtime_global_context.m_render_system->getSwapContext();
            swap_context.setMotorCameraVP(desired_mat);
        }
    }

    void CameraComponent::tickThirdPersonCamera(float delta_time)
    {
        unsigned int command = VKernel::g_runtime_global_context.m_input_system->getGameCommand();
        MotorComponent *motor_component = (VKernel::g_runtime_global_context.m_character_Manager->getCurrentCharacter())
                                              ->getObject()
                                              .lock()
                                              ->tryGetComponent(MotorComponent, "MotorComponent");

        if (VKernel::g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ||
            command < (unsigned int)VKernel::GameCommand::invalid || isFirst)
        {
            // get character
            std::shared_ptr<VKernel::Level> current_level =
                VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
            std::shared_ptr<VKernel::Character> current_character = current_level->getCurrentActiveCharacter().lock();
            if (current_character == nullptr)
                return;

            // Calculate cursor delta Q
            VKernel::Quaternion q_yaw, q_pitch;
            auto x = calculateCursorDeltaAngles(VKernel::g_runtime_global_context.m_input_system->m_cursor_delta_x,
                                                VKernel::g_runtime_global_context.m_input_system->m_cursor_delta_y);
            q_yaw.fromAngleAxis(x.first, VKernel::Vector3::NEGATIVE_UNIT_Y);
            q_pitch.fromAngleAxis(x.second, VKernel::Vector3::NEGATIVE_UNIT_X);

            // get
            VKernel::ThirdPersonCameraParameter *param = &(m_camera_res.m_third_camera);
            const float vertical_offset = param->m_vertical_offset;
            const float horizontal_offset = param->m_horizontal_offset;
            VKernel::Vector3 offset = VKernel::Vector3(0, vertical_offset, horizontal_offset);

            static VKernel::Quaternion last_pitch;
            static VKernel::Quaternion last_yaw;

            // set charactor new rotation

            if (VKernel::g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ||
                isButtonRight || isFirst)
            {
                // calculate camera new position
                m_position = motor_component->getTargetRotation() * param->m_cursor_pitch * offset +
                             std::static_pointer_cast<Games::ControlCabin>(current_character)
                                 ->getPosition(); ///< camera position: yaw * pitch * offset vector + charactor position

                // calculate camera new rotation
                VKernel::Vector3 center_pos =
                    std::static_pointer_cast<Games::ControlCabin>(current_character)->getPosition() +
                    VKernel::Vector3::UNIT_Y * vertical_offset; ///< look at target
                m_forward = center_pos - m_position;
                m_up = motor_component->getTargetRotation() * param->m_cursor_pitch *
                       VKernel::Vector3::NEGATIVE_UNIT_Y; ///<  yaw * pitch * Y vector
                m_left = m_up.crossProduct(m_forward);

                last_pitch = param->m_cursor_pitch;
                last_yaw = motor_component->getTargetRotation();

                isButtonRight = true;

                if (VKernel::g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
                {
                    // update new yaw and pitch
                    param->m_cursor_pitch = q_pitch * param->m_cursor_pitch;
                }
                else
                {
                    isButtonRight = false;
                }
                isFirst = false;
            }
            else
            {
                // calculate camera new position
                m_position = last_yaw * last_pitch * offset +
                             std::static_pointer_cast<Games::ControlCabin>(current_character)->getPosition();
            }

            // set swap data
            VKernel::Matrix4x4 desired_mat = VKernel::Math::makeLookAtMatrix(m_position, m_position + m_forward, m_up);
            VKernel::RenderSwapContext &swap_context =
                VKernel::g_runtime_global_context.m_render_system->getSwapContext();
            swap_context.setMotorCameraVP(desired_mat);
        }
    }
} // namespace Games
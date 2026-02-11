#include "runtime/Games/the_celestial_console/component/motor/motor_component.h"

#include "runtime/function/character/character.h"
#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/render/window_system.h"

#include "runtime/Games/the_celestial_console/control_cabin.h"

namespace Games
{
    void MotorComponent::postLoadResource(std::weak_ptr<VKernel::GObject> parent_object)
    {
        // set parent object
        m_parent_object = parent_object;

        // init target position
        VKernel::TransformComponent* transform_component =
            parent_object.lock()->tryGetComponent(VKernel::TransformComponent, "TransformComponent");

        m_target_position = transform_component->getPosition();
    }

    MotorComponent::~MotorComponent() {}

    void MotorComponent::tick(float delta_time) { tickPlayerMotor(delta_time); }

    void MotorComponent::tickPlayerMotor(float delta_time)
    {
        // Check if the associated object exists
        if (!m_parent_object.lock())
            return;

        // Check if the player exists
        std::shared_ptr<VKernel::Level> current_level =
            VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<VKernel::Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;
        if (current_character->getObjectID() != m_parent_object.lock()->getID())
            return;

        // get VKernel::TransformComponent
        VKernel::TransformComponent* transform_component =
            m_parent_object.lock()->tryGetComponent(VKernel::TransformComponent, "TransformComponent");

        // Check if it is in running mode
        unsigned int command = VKernel::g_runtime_global_context.m_input_system->getGameCommand();
        if (command >= (unsigned int)VKernel::GameCommand::invalid)
            return;

        // calculated target position and rotation
        calculatedDesiredHorizontalMoveSpeed(command, delta_time);
        calculatedDesiredVerticalMoveSpeed(command, delta_time);
        calculatedDesiredMoveDirection(command, transform_component->getRotation());
        calculateDesiredDisplacement(delta_time);
        calculateTargetPosition(transform_component->getPosition());

        if (VKernel::g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
        {
            VKernel::Quaternion q_yaw;
            q_yaw.fromAngleAxis(VKernel::g_runtime_global_context.m_input_system->m_cursor_delta_yaw,
                                VKernel::Vector3::NEGATIVE_UNIT_Y);
            m_character_rotation = q_yaw * m_character_rotation;
        }
    }

    void MotorComponent::calculatedDesiredHorizontalMoveSpeed(unsigned int command, float delta_time)
    {
        bool has_move_command =
            ((unsigned int)VKernel::GameCommand::forward | (unsigned int)VKernel::GameCommand::backward |
             (unsigned int)VKernel::GameCommand::left | (unsigned int)VKernel::GameCommand::right) &
            command;

        if (has_move_command)
        {
            bool has_sprint_command =
                (unsigned int)VKernel::GameCommand::sprint & command; ///< Was the shift key pressed

            float final_acceleration = m_motor_res.m_move_acceleration;
            float min_speed_ratio    = 0.f;
            float max_speed_ratio    = 0.f;
            max_speed_ratio          = m_motor_res.m_max_move_speed_ratio;
            if (has_sprint_command)
            {
                final_acceleration = m_motor_res.m_sprint_acceleration;
                min_speed_ratio    = m_motor_res.m_max_move_speed_ratio;
                max_speed_ratio    = m_motor_res.m_max_sprint_speed_ratio;
            }

            m_move_speed_ratio += final_acceleration * delta_time;
            m_move_speed_ratio = std::clamp(m_move_speed_ratio, min_speed_ratio, max_speed_ratio);
        }
        else
        {
            m_move_speed_ratio = 0.0f;
        }
    }

    void MotorComponent::calculatedDesiredVerticalMoveSpeed(unsigned int command, float delta_time)
    {
        if (m_motor_res.m_jump_height == 0.f)
            return;

        VKernel::Vector3 v_gravity {0.f, 0.f, -9.8f};
        const float      gravity = v_gravity.length();

        if (m_jump_state == JumpState::idle)
        {
            if ((unsigned int)VKernel::GameCommand::jump & command)
            {
                m_jump_state                  = JumpState::rising;
                m_vertical_move_speed         = VKernel::Math::sqrt(m_motor_res.m_jump_height * 2 * gravity);
                m_jump_horizontal_speed_ratio = m_move_speed_ratio;
            }
            else
            {
                m_vertical_move_speed = 0.f;
            }
        }
        else if (m_jump_state == JumpState::rising || m_jump_state == JumpState::falling)
        {
            m_vertical_move_speed -= gravity * delta_time;
            if (m_vertical_move_speed <= 0.f)
            {
                m_jump_state = JumpState::falling;
            }
        }
    }

    void MotorComponent::calculatedDesiredMoveDirection(unsigned int               command,
                                                        const VKernel::Quaternion& object_rotation)
    {
        {
            VKernel::Vector3 forward_dir = object_rotation * VKernel::Vector3::UNIT_Z;
            VKernel::Vector3 left_dir    = object_rotation * VKernel::Vector3::NEGATIVE_UNIT_X;

            if (command > 0)
            {
                m_desired_horizontal_move_direction = VKernel::Vector3::ZERO;
            }

            if ((unsigned int)VKernel::GameCommand::forward & command)
            {
                m_desired_horizontal_move_direction += forward_dir;
            }

            if ((unsigned int)VKernel::GameCommand::backward & command)
            {
                m_desired_horizontal_move_direction -= forward_dir;
            }

            if ((unsigned int)VKernel::GameCommand::left & command)
            {
                m_desired_horizontal_move_direction += left_dir;
            }

            if ((unsigned int)VKernel::GameCommand::right & command)
            {
                m_desired_horizontal_move_direction -= left_dir;
            }

            m_desired_horizontal_move_direction.normalise();
        }
    }

    void MotorComponent::calculateDesiredDisplacement(float delta_time)
    {
        m_desired_displacement =
            m_desired_horizontal_move_direction * m_motor_res.m_move_speed * m_move_speed_ratio * delta_time +
            VKernel::Vector3::NEGATIVE_UNIT_Y * m_vertical_move_speed * delta_time; ///< horizontal + vertical
    }

    void MotorComponent::calculateTargetPosition(const VKernel::Vector3&& current_position)
    {
        VKernel::Vector3 final_position;
        final_position    = current_position + m_desired_displacement;
        m_target_position = final_position;

        if (m_jump_state == JumpState::falling && final_position.y + m_desired_displacement.y >= 0.f) ///< stop falling
        {
            final_position.y = 0.f;
            m_jump_state     = JumpState::idle;
        }

        m_is_moving = (final_position - current_position).squaredLength() > 0.f;
    }

} // namespace Games
#include "runtime/function/framework/component/motor/motor_component.h"

#include "runtime/function/character/character.h"
#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/input/input_system.h"

namespace VKernel
{
    void MotorComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        // set parent object
        m_parent_object = parent_object;

        // init target position
        TransformComponent* transform_component =
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
        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;
        if (current_character->getObjectID() != m_parent_object.lock()->getID())
            return;

        // get TransformComponent
        TransformComponent* transform_component =
            m_parent_object.lock()->tryGetComponent(VKernel::TransformComponent, "TransformComponent");

        // Check if it is in running mode
        unsigned int command = g_runtime_global_context.m_input_system->getGameCommand();
        if (command >= (unsigned int)GameCommand::invalid)
            return;

        // calculated target position
        calculatedDesiredHorizontalMoveSpeed(command, delta_time);
        calculatedDesiredVerticalMoveSpeed(command, delta_time);
        calculatedDesiredMoveDirection(command, transform_component->getRotation());
        calculateDesiredDisplacement(delta_time);
        calculateTargetPosition(transform_component->getPosition());

        // update GO position
        transform_component->setPosition(m_target_position);
    }

    void MotorComponent::calculatedDesiredHorizontalMoveSpeed(unsigned int command, float delta_time)
    {
        bool has_move_command = ((unsigned int)GameCommand::forward | (unsigned int)GameCommand::backward |
                                 (unsigned int)GameCommand::left | (unsigned int)GameCommand::right) &
                                command;

        if (has_move_command)
        {
            m_move_speed_ratio = 1.0f;
        }
        else
        {
            m_move_speed_ratio = 0.0f;
        }
    }

    void MotorComponent::calculatedDesiredVerticalMoveSpeed(unsigned int command, float delta_time) {}

    void MotorComponent::calculatedDesiredMoveDirection(unsigned int command, const Quaternion& object_rotation)
    {
        {
            Vector3 forward_dir = object_rotation * Vector3::NEGATIVE_UNIT_Y;
            Vector3 left_dir    = object_rotation * Vector3::UNIT_X;

            if (command > 0)
            {
                m_desired_horizontal_move_direction = Vector3::ZERO;
            }

            if ((unsigned int)GameCommand::forward & command)
            {
                m_desired_horizontal_move_direction -= forward_dir;
            }

            if ((unsigned int)GameCommand::backward & command)
            {
                m_desired_horizontal_move_direction += forward_dir;
            }

            if ((unsigned int)GameCommand::left & command)
            {
                m_desired_horizontal_move_direction -= left_dir;
            }

            if ((unsigned int)GameCommand::right & command)
            {
                m_desired_horizontal_move_direction += left_dir;
            }

            m_desired_horizontal_move_direction.normalise();
        }
    }

    void MotorComponent::calculateDesiredDisplacement(float delta_time)
    {
        m_desired_displacement = m_desired_horizontal_move_direction * 1.0f * m_move_speed_ratio * delta_time;
    }

    void MotorComponent::calculateTargetPosition(const Vector3&& current_position)
    {
        Vector3 final_position;
        final_position    = current_position + m_desired_displacement;
        m_target_position = final_position;
    }

} // namespace VKernel
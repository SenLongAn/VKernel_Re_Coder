#include "runtime/Games/the_celestial_console/control_cabin.h"

#include "control_cabin.h"
#include "runtime/Games/the_celestial_console/component/motor/motor_component.h"
#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/window_system.h"

namespace Games
{
    CHARACTER_REGISTER(ControlCabin)

    ControlCabin::ControlCabin(std::shared_ptr<VKernel::GObject> character_object) { setObject(character_object); }

    void Games::ControlCabin::setCurrentCharacter() { Character::m_current_character = shared_from_this(); }

    void ControlCabin::tick(float delta_time)
    {
        // get component
        VKernel::TransformComponent* transform_component =
            m_character_object->tryGetComponent(VKernel::TransformComponent, "TransformComponent");
        MotorComponent* motor_component = m_character_object->tryGetComponent(MotorComponent, "MotorComponent");
        if (transform_component == nullptr || motor_component == nullptr)
        {
            return;
        }

        // set transform component position and rotation
        static bool                isFirst      = true;
        const VKernel::Quaternion& new_rotation = motor_component->getTargetRotation();
        if ((motor_component->getIsMoving() &&
             VKernel::g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)))
        {
            isFirst    = true;
            m_rotation = new_rotation;
            transform_component->setRotation(new_rotation);
        }
        else if (VKernel::g_runtime_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
        {
            isFirst    = true;
            m_rotation = new_rotation;
        }
        else if (motor_component->getIsMoving())
        {
            if (isFirst)
            {
                m_rotation = new_rotation;
                transform_component->setRotation(new_rotation);
            }
            isFirst = false;
        }

        const VKernel::Vector3& new_position = motor_component->getTargetPosition();
        m_position                           = new_position;
        transform_component->setPosition(new_position);
    }
} // namespace Games

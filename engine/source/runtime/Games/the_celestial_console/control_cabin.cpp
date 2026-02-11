#include "runtime/Games/the_celestial_console/control_cabin.h"

#include "runtime/Games/the_celestial_console/component/motor/motor_component.h"
#include "runtime/function/framework/component/transform/transform_component.h"

namespace Games
{
    CHARACTER_REGISTER(ControlCabin)

    ControlCabin::ControlCabin(std::shared_ptr<VKernel::GObject> character_object) { setObject(character_object); }

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

        // set transform component rotation
        if (motor_component->getIsMoving())
        {

            transform_component->setRotation(m_rotation);
        }

        // set character target position
        const VKernel::Vector3& new_position = motor_component->getTargetPosition();

        m_position = new_position;
    }
} // namespace Games
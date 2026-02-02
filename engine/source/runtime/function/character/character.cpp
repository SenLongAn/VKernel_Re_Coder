#include "runtime/function/character/character.h"

#include "runtime/function/framework/component/motor/motor_component.h"
#include "runtime/function/framework/component/transform/transform_component.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/window_system.h"
#include <GLFW/glfw3.h>

namespace VKernel
{
    Character::Character(std::shared_ptr<GObject> character_object) { setObject(character_object); }

    GObjectID Character::getObjectID() const
    {
        if (m_character_object)
        {
            return m_character_object->getID();
        }

        return k_invalid_gobject_id;
    }

    void Character::tick(float delta_time)
    {
        TransformComponent* transform_component =
            m_character_object->tryGetComponent(TransformComponent, "TransformComponent");
        MotorComponent* motor_component = m_character_object->tryGetComponent(MotorComponent, "MotorComponent");
        if (motor_component == nullptr)
        {
            return;
        }

        // set transform component rotation
        if (motor_component->getIsMoving())
        {

            transform_component->setRotation(m_rotation);
        }

        // set character target position
        const Vector3& new_position = motor_component->getTargetPosition();

        m_position = new_position;
    }

    void Character::setObject(std::shared_ptr<GObject> gobject) { m_character_object = gobject; }
} // namespace VKernel
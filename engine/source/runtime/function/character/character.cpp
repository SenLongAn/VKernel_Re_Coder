#include "runtime/function/character/character.h"

#include "runtime/function/framework/component/motor/motor_component.h"

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
        MotorComponent* motor_component = m_character_object->tryGetComponent(MotorComponent, "MotorComponent");
        if (motor_component == nullptr)
        {
            return;
        }

        const Vector3& new_position = motor_component->getTargetPosition(); // 获取目标位置

        m_position = new_position;
    }

    void Character::setObject(std::shared_ptr<GObject> gobject) { m_character_object = gobject; }
} // namespace VKernel
#include "runtime/Games/the_celestial_console/component/motor/auto_motor_component.h"

#include "runtime/function/character/character.h"
#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"

namespace Games
{
    void AutoMotorComponent::postLoadResource(std::weak_ptr<VKernel::GObject> parent_object)
    {
        // Set Associated Object
        m_parent_object = parent_object;
    }

    void AutoMotorComponent::tick(float delta_time)
    {
        // get
        if (!m_parent_object.lock())
            return;
        VKernel::TransformComponent* transform_component =
            m_parent_object.lock()->tryGetComponent(VKernel::TransformComponent, "TransformComponent");

        std::shared_ptr<VKernel::Level> current_level =
            VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<VKernel::Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;
        VKernel::TransformComponent* character_transform_component =
            (VKernel::g_runtime_global_context.m_character_Manager->getCurrentCharacter())
                ->getObject()
                .lock()
                ->tryGetComponent(VKernel::TransformComponent, "TransformComponent");

        transform_component->setPosition(VKernel::Vector3(character_transform_component->getPosition().x,
                                                          transform_component->getPosition().y,
                                                          character_transform_component->getPosition().z));
    }
} // namespace Games
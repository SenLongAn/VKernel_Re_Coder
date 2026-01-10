#include "runtime/function/framework/component/transform/transform_component.h"

namespace VKernel
{
    void TransformComponent::postLoadResource(std::weak_ptr<GObject> parent_gobject)
    {
        m_parent_object = parent_gobject;

        m_transform.updateRotation();
        // load data
        m_transform_buffer[0] = m_transform;
        m_transform_buffer[1] = m_transform;
    }

    void TransformComponent::setPosition(const Vector3& new_translation)
    {
        m_transform_buffer[m_next_index].setPosition(new_translation);
        m_transform.setPosition(new_translation);
    }

    void TransformComponent::setScale(const Vector3& new_scale)
    {
        m_transform_buffer[m_next_index].setScale(new_scale);
        m_transform.setScale(new_scale);
    }

    void TransformComponent::setRotation(const Quaternion& new_rotation)
    {
        m_transform_buffer[m_next_index].setRotation(new_rotation);
        m_transform.setRotation(new_rotation);
    }

    void TransformComponent::tick(float delta_time)
    {
        std::swap(m_current_index, m_next_index);

        m_transform_buffer[m_next_index] = m_transform;
    }

} // namespace VKernel
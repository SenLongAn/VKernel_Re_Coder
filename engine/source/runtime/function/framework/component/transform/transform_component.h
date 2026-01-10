#pragma once

#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/transform.h"

#include "runtime/function/framework/component/component.h"
#include "runtime/function/framework/object/object.h"

namespace VKernel
{
    REFLECTION_TYPE(TransformComponent)
    CLASS(TransformComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(TransformComponent)

    public:
        TransformComponent() = default; ///< Construction

        void postLoadResource(std::weak_ptr<GObject> parent_object) override; ///< load

        // get
        Vector3          getPosition() { return m_transform_buffer[m_current_index].getPosition(); }
        Vector3          getScale() { return m_transform_buffer[m_current_index].getScale(); }
        Quaternion       getRotation() { return m_transform_buffer[m_current_index].getRotation(); }
        const Transform& getTransformConst()
        {
            return m_transform_buffer[m_current_index];
        } ///< get current frame transform
        Transform& getTransform() { return m_transform_buffer[m_current_index]; } ///< get next frame transform
        Matrix4x4  getMatrix()
        {
            return m_transform_buffer[m_current_index].getMatrix();
        } ///< get current frame transform matrix

        // set
        void setPosition(const Vector3& new_translation);
        void setScale(const Vector3& new_scale);
        void setRotation(const Quaternion& new_rotation);

        // tick
        void tick(float delta_time) override;

    protected:
        META(Enable)
        Transform m_transform;

        Transform m_transform_buffer[2]; ///< Current and next frame transform, Double buffering prevents read-write
                                         ///< data conflicts
        size_t m_current_index {0};      ///< current frame index, read
        size_t m_next_index {1};         ///< next frame index, write
    };
} // namespace VKernel

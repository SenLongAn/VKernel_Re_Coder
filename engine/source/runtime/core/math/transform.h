#pragma once
#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/quaternion.h"
#include "runtime/core/math/vector3.h"

#include "runtime/core/meta/reflection/reflection.h"

/**
 * Transform：SRT
 */
namespace VKernel
{
    REFLECTION_TYPE(Transform)
    CLASS(Transform, WhiteListFields)
    {
        REFLECTION_BODY(Transform);

    public:
        META(Enable)
        Vector3 m_position {Vector3::ZERO};
        META(Enable)
        Vector3 m_scale {Vector3::UNIT_SCALE};
        META(Enable)
        Vector3 m_euler_rotation {Vector3::ZERO};
        META(Enable)
        bool m_is_serialization = false;

        // constructor
        //-----------------------------------------------------------------------
        Transform() = default;
        Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) :
            m_position {position}, m_scale {scale}, m_rotation {rotation}
        {}

        // get
        Vector3 getPosition() { return m_position; }
        Vector3 getScale() { return m_scale; }

        Quaternion getRotation()
        {
            if (m_is_serialization)
            {
                m_rotation = Quaternion(m_euler_rotation);
                return m_rotation;
            }
            else
            {
                return m_rotation;
            }
        }

        Matrix4x4 getMatrix()
        {
            if (m_is_serialization)
            {
                m_rotation = Quaternion(m_euler_rotation);
            }
            Matrix4x4 temp;
            temp.makeTransform(m_position, m_scale, m_rotation);
            return temp;
        }

        // set
        void setPosition(Vector3 position) { m_position = position; }
        void setScale(Vector3 scale) { m_scale = scale; }
        void setRotation(Vector3 euler_rotation) { m_rotation = Quaternion(euler_rotation); }
        void setRotation(Quaternion rotation) { m_rotation = rotation; }

    private:
        Quaternion m_rotation {Quaternion::IDENTITY};
    };
} // namespace VKernel

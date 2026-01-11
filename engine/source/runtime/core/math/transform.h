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

        Matrix4x4 getMatrix()
        {
            Matrix4x4 temp;
            temp.makeTransform(m_position, m_scale, m_rotation);
            return temp;
        }

        // update
        void updateRotation() { m_rotation = Quaternion(m_euler_rotation); };

        Quaternion m_rotation {Quaternion::IDENTITY};
    };
} // namespace VKernel

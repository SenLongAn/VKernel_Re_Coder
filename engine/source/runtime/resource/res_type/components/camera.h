#pragma once
#include "runtime/core/math/quaternion.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace VKernel
{
    REFLECTION_TYPE(CameraParameter)
    CLASS(CameraParameter, Fields) ///< basic class
    {
        REFLECTION_BODY(CameraParameter);

    public:
        float m_fov {50.f};

        virtual ~CameraParameter() {}
    };

    REFLECTION_TYPE(ThirdPersonCameraParameter)
    CLASS(ThirdPersonCameraParameter : public CameraParameter, WhiteListFields) ///< Derived class
    {
        REFLECTION_BODY(ThirdPersonCameraParameter);

    public:
        META(Enable)
        float m_horizontal_offset {3.f};
        META(Enable)
        float      m_vertical_offset {2.5f};
        Quaternion m_cursor_pitch;
        Quaternion m_cursor_yaw;
    };

    REFLECTION_TYPE(CameraComponentRes)
    CLASS(CameraComponentRes, Fields)
    {
        REFLECTION_BODY(CameraComponentRes);

    public:
        Reflection::ReflectionPtr<CameraParameter> m_parameter;

        CameraComponentRes() = default;
        CameraComponentRes(const CameraComponentRes& res);

        ~CameraComponentRes();
    };
} // namespace VKernel
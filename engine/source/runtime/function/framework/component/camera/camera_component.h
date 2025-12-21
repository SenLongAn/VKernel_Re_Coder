#pragma once

#include "runtime/function/framework/component/component.h"

#include "runtime/core/math/vector3.h"

/**
 * camera component
 */
namespace VKernel
{
    REFLECTION_TYPE(CameraComponent)
    CLASS(CameraComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(CameraComponent)
    public:
        CameraComponent() = default; ///< Destructor

        void postLoadResource(std::weak_ptr<GObject> parent_object) override; ///< load resource

        void tick(float delta_time) override; ///< tick

    private:
        // camera attribute
        Vector3 m_position {0.0f, 0.0f, 0.0f};
        Vector3 m_forward {Vector3::NEGATIVE_UNIT_Y};
        Vector3 m_up {Vector3::UNIT_Z};
        Vector3 m_left {Vector3::UNIT_X};
    };
} // namespace VKernel
#pragma once

#include "runtime/function/framework/component/component.h"

#include "runtime/core/math/vector3.h"

#include "runtime/function/render/render_object.h"
#include "runtime/resource/res_type/components/camera.h"

/**
 * camera component
 */
namespace VKernel
{
    enum class CameraMode : unsigned char ///< camera mode
    {
        third_person,
        first_person,
        invalid
    };

    REFLECTION_TYPE(CameraComponent)
    CLASS(CameraComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(CameraComponent)
    public:
        CameraComponent() = default; ///< Destructor

        void postLoadResource(std::weak_ptr<GObject> parent_object) override; ///< load resource

        void tick(float delta_time) override; ///< tick

    private:
        // update
        void tickFirstPersonCamera(float delta_time);
        void tickThirdPersonCamera(float delta_time);

    private:
        META(Enable)
        CameraComponentRes m_camera_res;

        CameraMode m_camera_mode {CameraMode::invalid}; ///< camera mode

        Vector3 m_position {0.0f, 0.0f, 0.0f}; ///< last frame camera position

        Vector3 m_forward {Vector3::UNIT_Z};
        Vector3 m_up {Vector3::NEGATIVE_UNIT_Y};
        Vector3 m_left {Vector3::UNIT_X};

        bool isFirst       = false;
        bool isButtonRight = false;
    };
} // namespace VKernel
#pragma once

#include "runtime/function/framework/component/component.h"

#include "runtime/core/math/vector3.h"

#include "runtime/function/render/render_object.h"
#include "runtime/resource/res_type/components/camera.h"

/**
 * camera component
 */
// namespace VKernel
// {
//     class Character;
//     class GObject;
//     class Component;
//     class Vector3;
//     class CameraComponentRes;
// } // namespace VKernel

namespace Games
{
    enum class CameraMode : unsigned char ///< camera mode
    {
        third_person,
        first_person,
        invalid
    };

    REFLECTION_TYPE(CameraComponent)
    CLASS(CameraComponent : public VKernel::Component, WhiteListFields)
    {
        REFLECTION_BODY(CameraComponent)
    public:
        META(Enable)
        VKernel::CameraComponentRes m_camera_res;

    public:
        CameraComponent() = default; ///< Destructor

        void postLoadResource(std::weak_ptr<VKernel::GObject> parent_object) override; ///< load resource

        void tick(float delta_time) override; ///< tick

    private:
        // update
        void tickFirstPersonCamera(float delta_time);
        void tickThirdPersonCamera(float delta_time);

    private:
        CameraMode m_camera_mode {CameraMode::third_person}; ///< camera mode
        CameraMode m_pre_camera_mode {CameraMode::invalid};  ///< camera mode

        VKernel::Vector3 m_position {0.0f, 0.0f, 0.0f}; ///< camera position

        VKernel::Vector3 m_forward {VKernel::Vector3::UNIT_Z}; ///< camera rotate
        VKernel::Vector3 m_up {VKernel::Vector3::NEGATIVE_UNIT_Y};
        VKernel::Vector3 m_left {VKernel::Vector3::UNIT_X};

        bool isButtonRight = false;
        bool isFirst       = true;
    };
} // namespace Games
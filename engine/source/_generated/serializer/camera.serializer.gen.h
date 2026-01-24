#pragma once
#include "runtime/resource/res_type/components/camera.h"

namespace VKernel{
    template<>
    Json Serializer::write(const CameraParameter& instance);
    template<>
    CameraParameter& Serializer::read(const Json& json_context, CameraParameter& instance);
    template<>
    Json Serializer::write(const FirstPersonCameraParameter& instance);
    template<>
    FirstPersonCameraParameter& Serializer::read(const Json& json_context, FirstPersonCameraParameter& instance);
    template<>
    Json Serializer::write(const ThirdPersonCameraParameter& instance);
    template<>
    ThirdPersonCameraParameter& Serializer::read(const Json& json_context, ThirdPersonCameraParameter& instance);
    template<>
    Json Serializer::write(const FreeCameraParameter& instance);
    template<>
    FreeCameraParameter& Serializer::read(const Json& json_context, FreeCameraParameter& instance);
    template<>
    Json Serializer::write(const CameraComponentRes& instance);
    template<>
    CameraComponentRes& Serializer::read(const Json& json_context, CameraComponentRes& instance);
}//namespace


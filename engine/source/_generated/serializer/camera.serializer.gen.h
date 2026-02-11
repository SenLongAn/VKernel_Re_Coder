#pragma once
#include "runtime/resource/res_type/components/camera.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::CameraParameter& instance);
    template<>
    VKernel::CameraParameter& Serializer::read(const Json& json_context, VKernel::CameraParameter& instance);
    template<>
    Json Serializer::write(const VKernel::FirstPersonCameraParameter& instance);
    template<>
    VKernel::FirstPersonCameraParameter& Serializer::read(const Json& json_context, VKernel::FirstPersonCameraParameter& instance);
    template<>
    Json Serializer::write(const VKernel::ThirdPersonCameraParameter& instance);
    template<>
    VKernel::ThirdPersonCameraParameter& Serializer::read(const Json& json_context, VKernel::ThirdPersonCameraParameter& instance);
    template<>
    Json Serializer::write(const VKernel::CameraComponentRes& instance);
    template<>
    VKernel::CameraComponentRes& Serializer::read(const Json& json_context, VKernel::CameraComponentRes& instance);
}//namespace


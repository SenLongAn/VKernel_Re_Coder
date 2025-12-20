#pragma once
#include "runtime/function/framework/component/camera/camera_component.h"
#include "_generated/serializer/component.serializer.gen.h"

namespace VKernel{
    template<>
    Json Serializer::write(const CameraComponent& instance);
    template<>
    CameraComponent& Serializer::read(const Json& json_context, CameraComponent& instance);
}//namespace


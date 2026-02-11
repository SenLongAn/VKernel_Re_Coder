#pragma once
#include "runtime/Games/the_celestial_console/component/camera/camera_component.h"
#include "_generated/serializer/component.serializer.gen.h"

namespace VKernel{
    template<>
    Json Serializer::write(const Games::CameraComponent& instance);
    template<>
    Games::CameraComponent& Serializer::read(const Json& json_context, Games::CameraComponent& instance);
}//namespace


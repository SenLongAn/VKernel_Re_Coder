#pragma once
#include "runtime/Games/the_celestial_console/component/motor/motor_component.h"
#include "_generated/serializer/component.serializer.gen.h"

namespace VKernel{
    template<>
    Json Serializer::write(const Games::MotorComponent& instance);
    template<>
    Games::MotorComponent& Serializer::read(const Json& json_context, Games::MotorComponent& instance);
}//namespace


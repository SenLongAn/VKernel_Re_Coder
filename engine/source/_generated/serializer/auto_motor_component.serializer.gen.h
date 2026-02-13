#pragma once
#include "runtime/Games/the_celestial_console/component/motor/auto_motor_component.h"
#include "_generated/serializer/component.serializer.gen.h"

namespace VKernel{
    template<>
    Json Serializer::write(const Games::AutoMotorComponent& instance);
    template<>
    Games::AutoMotorComponent& Serializer::read(const Json& json_context, Games::AutoMotorComponent& instance);
}//namespace


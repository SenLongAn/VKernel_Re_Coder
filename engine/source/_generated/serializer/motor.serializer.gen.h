#pragma once
#include "runtime/Games/the_celestial_console/component/motor/motor.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::MotorComponentRes& instance);
    template<>
    VKernel::MotorComponentRes& Serializer::read(const Json& json_context, VKernel::MotorComponentRes& instance);
}//namespace


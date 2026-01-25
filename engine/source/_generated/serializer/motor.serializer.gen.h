#pragma once
#include "runtime/resource/res_type/components/motor.h"

namespace VKernel{
    template<>
    Json Serializer::write(const MotorComponentRes& instance);
    template<>
    MotorComponentRes& Serializer::read(const Json& json_context, MotorComponentRes& instance);
}//namespace


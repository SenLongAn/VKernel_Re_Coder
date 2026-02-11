#pragma once
#include "runtime/resource/res_type/components/motor.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::MotorComponentRes& instance);
    template<>
    VKernel::MotorComponentRes& Serializer::read(const Json& json_context, VKernel::MotorComponentRes& instance);
}//namespace


#pragma once
#include "runtime/core/math/vector3.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::Vector3& instance);
    template<>
    VKernel::Vector3& Serializer::read(const Json& json_context, VKernel::Vector3& instance);
}//namespace


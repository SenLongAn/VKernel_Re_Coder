#pragma once
#include "runtime/core/math/transform.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::Transform& instance);
    template<>
    VKernel::Transform& Serializer::read(const Json& json_context, VKernel::Transform& instance);
}//namespace


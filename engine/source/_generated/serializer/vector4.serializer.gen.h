#pragma once
#include "runtime/core/math/vector4.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::Vector4& instance);
    template<>
    VKernel::Vector4& Serializer::read(const Json& json_context, VKernel::Vector4& instance);
}//namespace


#pragma once
#include "runtime/core/math/vector2.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::Vector2& instance);
    template<>
    VKernel::Vector2& Serializer::read(const Json& json_context, VKernel::Vector2& instance);
}//namespace


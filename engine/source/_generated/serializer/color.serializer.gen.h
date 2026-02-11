#pragma once
#include "runtime/core/color/color.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::Color& instance);
    template<>
    VKernel::Color& Serializer::read(const Json& json_context, VKernel::Color& instance);
}//namespace


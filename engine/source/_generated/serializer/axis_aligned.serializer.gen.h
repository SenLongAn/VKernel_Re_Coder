#pragma once
#include "runtime/core/math/axis_aligned.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::AxisAlignedBox& instance);
    template<>
    VKernel::AxisAlignedBox& Serializer::read(const Json& json_context, VKernel::AxisAlignedBox& instance);
}//namespace


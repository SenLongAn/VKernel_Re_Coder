#pragma once
#include "runtime/core/math/matrix4.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::Matrix4x4_& instance);
    template<>
    VKernel::Matrix4x4_& Serializer::read(const Json& json_context, VKernel::Matrix4x4_& instance);
}//namespace


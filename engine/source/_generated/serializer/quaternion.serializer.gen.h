#pragma once
#include "runtime/core/math/quaternion.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::Quaternion& instance);
    template<>
    VKernel::Quaternion& Serializer::read(const Json& json_context, VKernel::Quaternion& instance);
}//namespace


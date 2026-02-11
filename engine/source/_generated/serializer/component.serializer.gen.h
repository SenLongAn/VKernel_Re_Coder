#pragma once
#include "runtime/function/framework/component/component.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::Component& instance);
    template<>
    VKernel::Component& Serializer::read(const Json& json_context, VKernel::Component& instance);
}//namespace


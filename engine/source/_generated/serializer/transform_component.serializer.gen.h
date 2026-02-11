#pragma once
#include "runtime/function/framework/component/transform/transform_component.h"
#include "_generated/serializer/component.serializer.gen.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::TransformComponent& instance);
    template<>
    VKernel::TransformComponent& Serializer::read(const Json& json_context, VKernel::TransformComponent& instance);
}//namespace


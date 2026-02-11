#pragma once
#include "runtime/function/framework/component/mesh/mesh_component.h"
#include "_generated/serializer/component.serializer.gen.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::MeshComponent& instance);
    template<>
    VKernel::MeshComponent& Serializer::read(const Json& json_context, VKernel::MeshComponent& instance);
}//namespace


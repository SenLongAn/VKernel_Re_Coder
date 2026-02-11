#pragma once
#include "runtime/resource/res_type/components/mesh.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::SubMeshRes& instance);
    template<>
    VKernel::SubMeshRes& Serializer::read(const Json& json_context, VKernel::SubMeshRes& instance);
    template<>
    Json Serializer::write(const VKernel::MeshComponentRes& instance);
    template<>
    VKernel::MeshComponentRes& Serializer::read(const Json& json_context, VKernel::MeshComponentRes& instance);
}//namespace


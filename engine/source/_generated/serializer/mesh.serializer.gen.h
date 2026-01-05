#pragma once
#include "runtime/resource/res_type/components/mesh.h"

namespace VKernel{
    template<>
    Json Serializer::write(const SubMeshRes& instance);
    template<>
    SubMeshRes& Serializer::read(const Json& json_context, SubMeshRes& instance);
    template<>
    Json Serializer::write(const MeshComponentRes& instance);
    template<>
    MeshComponentRes& Serializer::read(const Json& json_context, MeshComponentRes& instance);
}//namespace


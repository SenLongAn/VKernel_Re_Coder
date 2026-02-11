#pragma once
#include "runtime/function/render/render_object.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::GameObjectMeshDesc& instance);
    template<>
    VKernel::GameObjectMeshDesc& Serializer::read(const Json& json_context, VKernel::GameObjectMeshDesc& instance);
    template<>
    Json Serializer::write(const VKernel::GameObjectMaterialDesc& instance);
    template<>
    VKernel::GameObjectMaterialDesc& Serializer::read(const Json& json_context, VKernel::GameObjectMaterialDesc& instance);
    template<>
    Json Serializer::write(const VKernel::GameObjectTransformDesc& instance);
    template<>
    VKernel::GameObjectTransformDesc& Serializer::read(const Json& json_context, VKernel::GameObjectTransformDesc& instance);
    template<>
    Json Serializer::write(const VKernel::GameObjectPartDesc& instance);
    template<>
    VKernel::GameObjectPartDesc& Serializer::read(const Json& json_context, VKernel::GameObjectPartDesc& instance);
}//namespace


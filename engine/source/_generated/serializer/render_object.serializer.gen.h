#pragma once
#include "runtime/function/render/render_object.h"

namespace VKernel{
    template<>
    Json Serializer::write(const GameObjectMeshDesc& instance);
    template<>
    GameObjectMeshDesc& Serializer::read(const Json& json_context, GameObjectMeshDesc& instance);
    template<>
    Json Serializer::write(const GameObjectMaterialDesc& instance);
    template<>
    GameObjectMaterialDesc& Serializer::read(const Json& json_context, GameObjectMaterialDesc& instance);
    template<>
    Json Serializer::write(const GameObjectTransformDesc& instance);
    template<>
    GameObjectTransformDesc& Serializer::read(const Json& json_context, GameObjectTransformDesc& instance);
    template<>
    Json Serializer::write(const GameObjectPartDesc& instance);
    template<>
    GameObjectPartDesc& Serializer::read(const Json& json_context, GameObjectPartDesc& instance);
}//namespace


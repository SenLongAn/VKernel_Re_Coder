#pragma once
#include "runtime/resource/res_type/common/object.h"

namespace VKernel{
    template<>
    Json Serializer::write(const ComponentDefinitionRes& instance);
    template<>
    ComponentDefinitionRes& Serializer::read(const Json& json_context, ComponentDefinitionRes& instance);
    template<>
    Json Serializer::write(const ObjectDefinitionRes& instance);
    template<>
    ObjectDefinitionRes& Serializer::read(const Json& json_context, ObjectDefinitionRes& instance);
    template<>
    Json Serializer::write(const ObjectInstanceRes& instance);
    template<>
    ObjectInstanceRes& Serializer::read(const Json& json_context, ObjectInstanceRes& instance);
}//namespace


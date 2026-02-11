#pragma once
#include "runtime/resource/res_type/common/object.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::ComponentDefinitionRes& instance);
    template<>
    VKernel::ComponentDefinitionRes& Serializer::read(const Json& json_context, VKernel::ComponentDefinitionRes& instance);
    template<>
    Json Serializer::write(const VKernel::ObjectDefinitionRes& instance);
    template<>
    VKernel::ObjectDefinitionRes& Serializer::read(const Json& json_context, VKernel::ObjectDefinitionRes& instance);
    template<>
    Json Serializer::write(const VKernel::ObjectInstanceRes& instance);
    template<>
    VKernel::ObjectInstanceRes& Serializer::read(const Json& json_context, VKernel::ObjectInstanceRes& instance);
}//namespace


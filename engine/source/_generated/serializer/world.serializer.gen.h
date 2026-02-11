#pragma once
#include "runtime/resource/res_type/common/world.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::WorldRes& instance);
    template<>
    VKernel::WorldRes& Serializer::read(const Json& json_context, VKernel::WorldRes& instance);
}//namespace


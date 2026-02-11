#pragma once
#include "runtime/resource/res_type/common/level.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::LevelRes& instance);
    template<>
    VKernel::LevelRes& Serializer::read(const Json& json_context, VKernel::LevelRes& instance);
}//namespace


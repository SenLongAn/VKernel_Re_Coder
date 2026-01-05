#pragma once
#include "runtime/resource/res_type/common/level.h"

namespace VKernel{
    template<>
    Json Serializer::write(const LevelRes& instance);
    template<>
    LevelRes& Serializer::read(const Json& json_context, LevelRes& instance);
}//namespace


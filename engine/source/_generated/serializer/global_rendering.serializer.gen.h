#pragma once
#include "runtime/resource/res_type/global/global_rendering.h"

namespace VKernel{
    template<>
    Json Serializer::write(const GlobalRenderingRes& instance);
    template<>
    GlobalRenderingRes& Serializer::read(const Json& json_context, GlobalRenderingRes& instance);
}//namespace


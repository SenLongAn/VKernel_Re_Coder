#pragma once
#include "runtime/resource/res_type/data/material.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::MaterialRes& instance);
    template<>
    VKernel::MaterialRes& Serializer::read(const Json& json_context, VKernel::MaterialRes& instance);
}//namespace


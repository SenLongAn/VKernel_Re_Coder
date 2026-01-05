#pragma once
#include "runtime/core/meta/reflection/reflection.h"

namespace VKernel
{
    REFLECTION_TYPE(MaterialRes)
    CLASS(MaterialRes, Fields)
    {
        REFLECTION_BODY(MaterialRes);

    public:
        std::string m_base_colour_texture_file;
        std::string m_normal_texture_file;
    };
} // namespace VKernel
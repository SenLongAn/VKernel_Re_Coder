#pragma once

#include "runtime/core/math/matrix4.h"

#include <cstdint>
#include <vector>

/**
 * entity struct
 */
namespace VKernel
{
    class RenderEntity
    {
    public:
        uint32_t  m_instance_id {0};
        Matrix4x4 m_model_matrix {Matrix4x4::IDENTITY};
    };
} // namespace VKernel

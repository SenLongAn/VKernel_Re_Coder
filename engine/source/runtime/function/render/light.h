#pragma once

#include "runtime/core/math/vector3.h"

/**
 * light
 */

namespace VKernel
{
    struct AmbientLight
    {
        Vector3 m_irradiance;
    };

    struct PDirectionalLight
    {
        Vector3 m_direction;
        Vector3 m_color;
    };

} // namespace VKernel
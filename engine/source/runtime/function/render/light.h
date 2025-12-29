#pragma once

#include "runtime/core/math/vector3.h"

/**
 * light
 */

namespace VKernel
{
    struct AmbientLight ///< Ambient Light
    {
        Vector3 m_irradiance;
    };

    struct PDirectionalLight ///< DirectionalLight
    {
        Vector3 m_direction;
        Vector3 m_color;
    };

    struct PointLight ///< point light
    {
        Vector3 m_position;
        Vector3 m_flux; ///< Light energy

        // Calculate attenuation radius
        float calculateRadius() const
        {
            const float INTENSITY_CUTOFF    = 1.0f;
            const float ATTENTUATION_CUTOFF = 0.05f;
            Vector3     intensity           = m_flux / (4.0f * Math_PI);
            float       maxIntensity        = Vector3::getMaxElement(intensity);
            float       attenuation = Math::max(INTENSITY_CUTOFF, ATTENTUATION_CUTOFF * maxIntensity) / maxIntensity;
            return 1.0f / sqrtf(attenuation);
        }
    };

    class PointLightList
    {
    public:
        std::vector<PointLight> m_lights; ///< point light list
    };

} // namespace VKernel
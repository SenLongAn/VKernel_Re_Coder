#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

/**
 * basic math
 */
namespace VKernel
{
    static const float Float_EPSILON  = FLT_EPSILON; ///< The difference between 1.0 and the smallest float greater than 1.0
    static const float Double_EPSILON = DBL_EPSILON; ///< The difference between 1.0 and the smallest double greater than 1.0

    class Math
    {
    public:
        Math();

        static float abs(float value) { return std::fabs(value); } ///< absolute value
        static bool  isNan(float f) { return std::isnan(f); } ///< Is it a non-numeric value
        static float sqr(float value) { return value * value; } ///< square
        static float sqrt(float fValue) { return std::sqrt(fValue); } ///< square root
        static float invSqrt(float value) { return 1.f / sqrt(value); } ///< inverse square root
        static bool  realEqual(float a, float b, float tolerance = std::numeric_limits<float>::epsilon()); ///< float number equality comparison
        static float clamp(float v, float min, float max) { return std::clamp(v, min, max); } ///< Limit v within the range of min and max.
        static float getMaxElement(float x, float y, float z) { return std::max({x, y, z}); } ///< Obtain the maximum value in xyz
    };
}
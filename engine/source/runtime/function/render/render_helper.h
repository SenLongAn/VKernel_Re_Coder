#pragma once

#include "runtime/core/math/vector3.h"
#include "runtime/core/math/vector4.h"

/**
 * Utility functions
 */
namespace VKernel
{
    class RenderScene;
    class RenderCamera;

    static inline uint32_t roundUp(uint32_t value, uint32_t alignment) ///< Align value to alignment
    {
        uint32_t temp = value + alignment - static_cast<uint32_t>(1);
        return (temp - temp % alignment);
    }

    struct BoundingBox ///< BoundingBox
    {
        Vector3 min_bound {std::numeric_limits<float>::max(),
                           std::numeric_limits<float>::max(),
                           std::numeric_limits<float>::max()}; ///< Initially set to an invalid value
        Vector3 max_bound {std::numeric_limits<float>::min(),
                           std::numeric_limits<float>::min(),
                           std::numeric_limits<float>::min()};

        BoundingBox() {}

        // Accept min and max points
        BoundingBox(const Vector3& minv, const Vector3& maxv)
        {
            min_bound = minv;
            max_bound = maxv;
        }

        // merge
        void merge(const BoundingBox& rhs)
        {
            min_bound.makeFloor(rhs.min_bound); // Take the minimum of each component
            max_bound.makeCeil(rhs.max_bound);  // Take the maximum of each component
        }

        void merge(const Vector3& point)
        {
            min_bound.makeFloor(point);
            max_bound.makeCeil(point);
        }
    };

    BoundingBox BoundingBoxTransform(BoundingBox const& b, Matrix4x4 const& m); ///< Apply Transform to BoundingBox

    Matrix4x4 CalculateDirectionalLightCamera(
        RenderScene&  scene,
        RenderCamera& camera); // Calculate the view-projection matrix of the directional light

    std::vector<std::vector<Matrix4x4>> CalculatePointLightCamera(RenderScene& scene);
} // namespace VKernel
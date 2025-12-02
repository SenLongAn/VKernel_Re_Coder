# pragma once

#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/vector3.h"

/**
 * Render the required data portions
 */
namespace VKernel
{
    struct MeshPerframeStorageBufferObject
    {
        Matrix4x4 proj_view_matrix;
        Vector3 camera_position;
    };
}
#pragma once

#include "runtime/core/math/axis_aligned.h"
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
        // mesh
        uint32_t  m_mesh_id {0};
        Matrix4x4 m_model_matrix {Matrix4x4::IDENTITY};

        // material
        size_t m_material_asset_id {0};

        // bounding box(local)
        AxisAlignedBox m_bounding_box;
    };
} // namespace VKernel

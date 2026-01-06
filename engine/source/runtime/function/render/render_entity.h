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
        uint32_t m_instance_id {0};       ///< submesh guid
        uint32_t m_mesh_asset_id {0};     ///< mesh guid
        size_t   m_material_asset_id {0}; ///< material guid

        // mesh
        Matrix4x4 m_model_matrix {Matrix4x4::IDENTITY};

        // material

        // bounding box(local)
        AxisAlignedBox m_bounding_box;
    };
} // namespace VKernel

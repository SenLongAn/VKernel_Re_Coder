#pragma once

#include "runtime/core/math/transform.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace VKernel
{
    REFLECTION_TYPE(SubMeshRes)
    CLASS(SubMeshRes, Fields)
    {
        REFLECTION_BODY(SubMeshRes);

    public:
        Vector3            m_color = Vector3::UNIT_SCALE;
        std::string        m_obj_file_ref; // OBJ file path
        VKernel::Transform m_transform;    // Relative transformation
        std::string        m_material;     // Material JSON file path
    };

    REFLECTION_TYPE(MeshComponentRes)
    CLASS(MeshComponentRes, Fields)
    {
        REFLECTION_BODY(MeshComponentRes);

    public:
        std::vector<SubMeshRes> m_sub_meshes; ///< all SubMesh
    };
} // namespace VKernel
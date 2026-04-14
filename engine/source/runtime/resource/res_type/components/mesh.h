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
        std::string m_obj_file_ref;     // OBJ file path
        VKernel::Transform m_transform; // Relative transformation
        std::string m_material;         // Material JSON file path
    };

    REFLECTION_TYPE(MeshComponentRes)
    CLASS(MeshComponentRes, Fields)
    {
        REFLECTION_BODY(MeshComponentRes);

    public:
        Vector3 m_color = Vector3::UNIT_SCALE;
        bool m_apply_lighting = false;
        bool m_apply_texture = false;
        std::string m_name = "";
        std::string m_introduction = "";
        std::string m_type = "";
        std::vector<SubMeshRes> m_sub_meshes; ///< all SubMesh
    };
} // namespace VKernel
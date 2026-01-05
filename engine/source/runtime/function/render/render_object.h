#pragma once

#include "runtime/core/math/matrix4.h"

#include <string>
#include <vector>

namespace VKernel
{
    REFLECTION_TYPE(GameObjectMeshDesc)
    STRUCT(GameObjectMeshDesc, Fields)
    {
        REFLECTION_BODY(GameObjectMeshDesc)

        // file path
        std::string m_mesh_file;
    };

    REFLECTION_TYPE(GameObjectMaterialDesc)
    STRUCT(GameObjectMaterialDesc, Fields)
    {
        REFLECTION_BODY(GameObjectMaterialDesc)

        // file path
        std::string m_base_color_texture_file;
        std::string m_normal_texture_file;
        bool        m_with_texture {false};
    };

    REFLECTION_TYPE(GameObjectTransformDesc)
    STRUCT(GameObjectTransformDesc, WhiteListFields)
    {
        REFLECTION_BODY(GameObjectTransformDesc)
        Matrix4x4 m_transform_matrix {Matrix4x4::IDENTITY};
    };

    REFLECTION_TYPE(GameObjectPartDesc)
    STRUCT(GameObjectPartDesc, Fields)
    {
        REFLECTION_BODY(GameObjectPartDesc)

        // file path
        GameObjectMeshDesc     m_mesh_desc;
        GameObjectMaterialDesc m_material_desc;

        GameObjectTransformDesc m_transform_desc; ///< Relative transformation
    };

    class GameObjectDesc
    {
    public:
        GameObjectDesc() {}
        GameObjectDesc(const std::vector<GameObjectPartDesc>& parts) : m_object_parts(parts) {}

        const std::vector<GameObjectPartDesc>& getObjectParts() const { return m_object_parts; }

    private:
        std::vector<GameObjectPartDesc> m_object_parts; ///< all submesh
    };
} // namespace VKernel
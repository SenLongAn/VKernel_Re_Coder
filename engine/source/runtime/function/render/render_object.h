#pragma once

#include "runtime/core/math/matrix4.h"
#include "runtime/function/framework/object/object_id_allocator.h"

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
        Vector3                 m_color          = Vector3::UNIT_SCALE;
        Vector3                 m_apply_lighting = Vector3::ZERO;
        int                     m_apply_texture  = 0;
    };

    constexpr size_t k_invalid_part_id = std::numeric_limits<size_t>::max(); ///< invalid value

    struct GameObjectPartId
    {
        GObjectID m_go_id {k_invalid_gobject_id}; ///< Gobject id
        size_t    m_part_id {k_invalid_part_id};  ///< submesh index

        bool operator==(const GameObjectPartId& rhs) const ///< operator ==
        {
            return m_go_id == rhs.m_go_id && m_part_id == rhs.m_part_id; ///<
        }
        size_t getHashValue() const { return m_go_id ^ (m_part_id << 1); }
        bool isValid() const { return m_go_id != k_invalid_gobject_id && m_part_id != k_invalid_part_id; } ///< is valid
    };

    class GameObjectDesc
    {
    public:
        GameObjectDesc() : m_go_id(0) {}
        GameObjectDesc(size_t go_id, const std::vector<GameObjectPartDesc>& parts) :
            m_go_id(go_id), m_object_parts(parts)
        {}

        GObjectID                              getId() const { return m_go_id; }
        const std::vector<GameObjectPartDesc>& getObjectParts() const { return m_object_parts; }

    private:
        GObjectID                       m_go_id {k_invalid_gobject_id}; ///< guid
        std::vector<GameObjectPartDesc> m_object_parts;                 ///< all submesh
    };
} // namespace VKernel

template<>
struct std::hash<VKernel::GameObjectPartId>
{
    size_t operator()(const VKernel::GameObjectPartId& rhs) const noexcept { return rhs.getHashValue(); }
};

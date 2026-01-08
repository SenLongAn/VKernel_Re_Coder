#pragma once

#include "runtime/function/render/render_entity.h"
#include "runtime/function/render/render_type.h"

/**
 * Gizmo data: vertex data and buffer
 */
namespace VKernel
{
    class EditorTranslationAxis : public RenderEntity ///< Translation
    {
    public:
        EditorTranslationAxis();
        RenderMeshData m_mesh_data;
    };

    class EditorRotationAxis : public RenderEntity ///< Rotation
    {
    public:
        EditorRotationAxis();
        RenderMeshData m_mesh_data;
    };

    class EditorScaleAxis : public RenderEntity ///< Scale
    {
    public:
        EditorScaleAxis();
        RenderMeshData m_mesh_data;
    };
} // namespace VKernel

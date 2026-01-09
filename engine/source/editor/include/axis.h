#pragma once

#include "runtime/function/render/render_entity.h"
#include "runtime/function/render/render_type.h"

/**
 * Gizmo data: vertex data and buffer
 */
namespace ReCoder
{
    class EditorTranslationAxis : public VKernel::RenderEntity ///< Translation
    {
    public:
        EditorTranslationAxis();
        VKernel::RenderMeshData m_mesh_data;
    };

    class EditorRotationAxis : public VKernel::RenderEntity ///< Rotation
    {
    public:
        EditorRotationAxis();
        VKernel::RenderMeshData m_mesh_data;
    };

    class EditorScaleAxis : public VKernel::RenderEntity ///< Scale
    {
    public:
        EditorScaleAxis();
        VKernel::RenderMeshData m_mesh_data;
    };
} // namespace ReCoder

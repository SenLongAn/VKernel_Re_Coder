#pragma once

#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/vector2.h"
#include "runtime/function/framework/object/object.h"

#include "editor/include/axis.h"

#include <memory>

namespace VKernel
{
    class RenderCamera;
}
/**
 * editor manager
 */
namespace ReCoder
{
    enum class EditorAxisMode : int ///< axis mode
    {
        TranslateMode = 0,
        RotateMode    = 1,
        ScaleMode     = 2,
        Default       = 3
    };

    class EditorSceneManager
    {
    public:
        void initialize(); ///< init

        // set
        void setEditorCamera(std::shared_ptr<VKernel::RenderCamera> camera) { m_camera = camera; }
        void setSelectedObjectID(VKernel::GObjectID selected_gobject_id)
        {
            m_selected_gobject_id = selected_gobject_id;
        };
        void setEditorAxisMode(EditorAxisMode new_axis_mode) { m_axis_mode = new_axis_mode; }
        void setSelectedObjectMatrix(VKernel::Matrix4x4 new_object_matrix)
        {
            m_selected_object_matrix = new_object_matrix;
        }

        // get
        std::shared_ptr<VKernel::RenderCamera> getEditorCamera() { return m_camera; };
        std::pair<uint32_t, VKernel::Vector4>  getGuidOfPickedMesh(const VKernel::Vector2& picked_uv) const;
        VKernel::GObjectID                     getSelectedObjectID() { return m_selected_gobject_id; };
        std::weak_ptr<VKernel::GObject>        getSelectedGObject() const;
        EditorAxisMode                         getEditorAxisMode() { return m_axis_mode; }
        VKernel::RenderEntity*
        getAxisMeshByType(EditorAxisMode axis_mode); ///< Return the axis object according to the axis mode
        VKernel::Matrix4x4 getSelectedObjectMatrix() { return m_selected_object_matrix; }

        // update object
        void   onGObjectSelected(VKernel::GObjectID selected_gobject_id);
        void   uploadAxisResource();     ///< Allocate entity ID and fill the vertex buffer
        void   drawSelectedEntityAxis(); ///< update axis Model matrix
        size_t updateCursorOnAxis(
            VKernel::Vector2 cursor_uv,
            VKernel::Vector2 game_engine_window_size); ///< Calculate which axis the mouse is hovering over
        void moveEntity(float              new_mouse_pos_x,
                        float              new_mouse_pos_y,
                        float              last_mouse_pos_x,
                        float              last_mouse_pos_y,
                        VKernel::Vector2   engine_window_pos,
                        VKernel::Vector2   engine_window_size,
                        size_t             cursor_on_axis,
                        VKernel::Matrix4x4 model_matrix); ///< move Entity

    private:
        std::shared_ptr<VKernel::RenderCamera> m_camera;

        VKernel::GObjectID m_selected_gobject_id {VKernel::k_invalid_gobject_id};   ///< selected GO id
        VKernel::Matrix4x4 m_selected_object_matrix {VKernel::Matrix4x4::IDENTITY}; ///< selected GO matrix

        // gizmo axis
        EditorTranslationAxis m_translation_axis; ///< axis object
        EditorRotationAxis    m_rotation_axis;
        EditorScaleAxis       m_scale_aixs;

        EditorAxisMode m_axis_mode {EditorAxisMode::TranslateMode}; ///< current axis mode

        size_t m_selected_axis {3}; ///< current Axis index of hovered mouse

        bool m_is_show_axis = true; ///< Should the axis be displayed
    };
} // namespace ReCoder
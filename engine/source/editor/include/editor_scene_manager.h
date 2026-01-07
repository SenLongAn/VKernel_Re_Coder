#pragma once

#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/vector2.h"
#include "runtime/function/framework/object/object.h"


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

        // get
        std::shared_ptr<VKernel::RenderCamera> getEditorCamera() { return m_camera; };
        size_t                                 getGuidOfPickedMesh(const VKernel::Vector2& picked_uv) const;
        VKernel::GObjectID                     getSelectedObjectID() { return m_selected_gobject_id; };
        std::weak_ptr<VKernel::GObject>        getSelectedGObject() const;

        // update object
        void onGObjectSelected(VKernel::GObjectID selected_gobject_id);

    private:
        std::shared_ptr<VKernel::RenderCamera> m_camera;

        VKernel::GObjectID m_selected_gobject_id {VKernel::k_invalid_gobject_id};   ///< selected GO id
        VKernel::Matrix4x4 m_selected_object_matrix {VKernel::Matrix4x4::IDENTITY}; ///< selected GO matrix
    };
} // namespace ReCoder
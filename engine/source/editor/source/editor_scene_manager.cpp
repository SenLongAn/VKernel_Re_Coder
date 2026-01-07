#include "editor/include/editor_scene_manager.h"

#include "editor/include/editor_global_context.h"
#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"

#include <iostream>
namespace ReCoder
{
    void EditorSceneManager::initialize() {}

    size_t EditorSceneManager::getGuidOfPickedMesh(const VKernel::Vector2& picked_uv) const
    {
        return g_editor_global_context.m_render_system->getGuidOfPickedMesh(picked_uv);
    }

    std::weak_ptr<VKernel::GObject> EditorSceneManager::getSelectedGObject() const
    {
        std::weak_ptr<VKernel::GObject> selected_object;
        if (m_selected_gobject_id != VKernel::k_invalid_gobject_id) ///< If the id is valid
        {
            // Get object from level
            std::shared_ptr<VKernel::Level> level =
                VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
            if (level != nullptr)
            {
                selected_object = level->getGObjectByID(m_selected_gobject_id);
            }
        }
        return selected_object;
    }

    void EditorSceneManager::onGObjectSelected(VKernel::GObjectID selected_gobject_id)
    {
        // update selected GO id
        if (selected_gobject_id == m_selected_gobject_id)
            return;

        m_selected_gobject_id = selected_gobject_id;

        // get GO
        std::shared_ptr<VKernel::GObject> selected_gobject = getSelectedGObject().lock();
        if (selected_gobject)
        {
            // get TransformComponent matrix
            VKernel::TransformComponent* transform_component =
                selected_gobject->tryGetComponent(VKernel::TransformComponent, "TransformComponent");
            m_selected_object_matrix = transform_component->getMatrix();
        }

        // drawSelectedEntityAxis();
    }
} // namespace ReCoder

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

    VKernel::RenderEntity* EditorSceneManager::getAxisMeshByType(EditorAxisMode axis_mode)
    {
        VKernel::RenderEntity* axis_mesh = nullptr; ///< base class
        switch (axis_mode)
        {
            case EditorAxisMode::TranslateMode:
                axis_mesh = &m_translation_axis;
                break;
            case EditorAxisMode::RotateMode:
                axis_mesh = &m_rotation_axis;
                break;
            case EditorAxisMode::ScaleMode:
                axis_mesh = &m_scale_aixs;
                break;
            default:
                break;
        }
        return axis_mesh; ///< Derived class
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

        // update axis Model matrix
        drawSelectedEntityAxis();
    }

    void EditorSceneManager::uploadAxisResource()
    {
        // get guid allocator
        auto& instance_id_allocator   = g_editor_global_context.m_render_system->getGOInstanceIdAllocator();
        auto& mesh_asset_id_allocator = g_editor_global_context.m_render_system->getMeshAssetIdAllocator();

        // assign some value that won't be used by other game objects
        {
            VKernel::GameObjectPartId axis_instance_id = {0xFFAA, 0xFFAA};
            VKernel::MeshSourceDesc   mesh_source_desc = {"%%translation_axis%%"};

            m_translation_axis.m_instance_id   = instance_id_allocator.allocGuid(axis_instance_id);
            m_translation_axis.m_mesh_asset_id = mesh_asset_id_allocator.allocGuid(mesh_source_desc);
        }

        {
            VKernel::GameObjectPartId axis_instance_id = {0xFFBB, 0xFFBB};
            VKernel::MeshSourceDesc   mesh_source_desc = {"%%rotate_axis%%"};

            m_rotation_axis.m_instance_id   = instance_id_allocator.allocGuid(axis_instance_id);
            m_rotation_axis.m_mesh_asset_id = mesh_asset_id_allocator.allocGuid(mesh_source_desc);
        }

        {
            VKernel::GameObjectPartId axis_instance_id = {0xFFCC, 0xFFCC};
            VKernel::MeshSourceDesc   mesh_source_desc = {"%%scale_axis%%"};

            m_scale_aixs.m_instance_id   = instance_id_allocator.allocGuid(axis_instance_id);
            m_scale_aixs.m_mesh_asset_id = mesh_asset_id_allocator.allocGuid(mesh_source_desc);
        }

        //
        g_editor_global_context.m_render_system->createAxis(
            {m_translation_axis, m_rotation_axis, m_scale_aixs},
            {m_translation_axis.m_mesh_data, m_rotation_axis.m_mesh_data, m_scale_aixs.m_mesh_data});
    }

    void EditorSceneManager::drawSelectedEntityAxis()
    {
        std::shared_ptr<VKernel::GObject> selected_object = getSelectedGObject().lock(); ///< get selected object

        if (selected_object != nullptr)
        {
            // get selected object TransformComponent
            VKernel::TransformComponent* transform_component =
                selected_object->tryGetComponent(VKernel::TransformComponent, "TransformComponent");

            // decomposition
            VKernel::Vector3    scale;
            VKernel::Quaternion rotation;
            VKernel::Vector3    translation;
            transform_component->getMatrix().decomposition(translation, scale, rotation);

            // The model matrix of the axis
            VKernel::Matrix4x4 translation_matrix = VKernel::Matrix4x4::getTrans(translation);
            VKernel::Matrix4x4 scale_matrix       = VKernel::Matrix4x4::buildScaleMatrix(1.0f, 1.0f, 1.0f);
            VKernel::Matrix4x4 axis_model_matrix  = translation_matrix * scale_matrix;

            VKernel::RenderEntity* selected_aixs = getAxisMeshByType(m_axis_mode); ///< get axis object

            if (m_axis_mode == EditorAxisMode::TranslateMode || m_axis_mode == EditorAxisMode::RotateMode)
            {
                selected_aixs->m_model_matrix = axis_model_matrix; ///< update
            }
            else if (m_axis_mode == EditorAxisMode::ScaleMode)
            {
                selected_aixs->m_model_matrix = axis_model_matrix * VKernel::Matrix4x4(rotation);
            }

            g_editor_global_context.m_render_system->setVisibleAxis(*selected_aixs); ///< set Visible
        }
        else
        {
            g_editor_global_context.m_render_system->setVisibleAxis(std::nullopt);
        }
    }
} // namespace ReCoder

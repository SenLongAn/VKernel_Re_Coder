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

    float intersectPlaneRay(VKernel::Vector3 normal, float d, VKernel::Vector3 origin, VKernel::Vector3 dir)
    {
        float deno = normal.dotProduct(dir);
        if (fabs(deno) < 0.0001)
        {
            deno = 0.0001;
        }

        return -(normal.dotProduct(origin) + d) / deno;
    }

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

    size_t EditorSceneManager::updateCursorOnAxis(VKernel::Vector2 cursor_uv, VKernel::Vector2 game_engine_window_size)
    {

        // get camera data
        float            camera_fov      = m_camera->getFovYDeprecated();
        VKernel::Vector3 camera_forward  = m_camera->forward();
        VKernel::Vector3 camera_up       = m_camera->up();
        VKernel::Vector3 camera_right    = m_camera->right();
        VKernel::Vector3 camera_position = m_camera->position();

        // Is the object selected
        if (m_selected_gobject_id == VKernel::k_invalid_gobject_id)
        {
            return m_selected_axis;
        }

        // get axis object
        VKernel::RenderEntity* selected_aixs = getAxisMeshByType(m_axis_mode);

        m_selected_axis = 3;
        if (m_is_show_axis == false)
        {
            return m_selected_axis;
        }
        else
        {
            // decomposition
            VKernel::Matrix4x4  model_matrix = selected_aixs->m_model_matrix;
            VKernel::Vector3    model_scale;
            VKernel::Quaternion model_rotation;
            VKernel::Vector3    model_translation;
            model_matrix.decomposition(model_translation, model_scale, model_rotation);

            // uv -> world ray
            float window_forward = game_engine_window_size.y / 2.0f /
                                   VKernel::Math::tan(VKernel::Math::degreesToRadians(camera_fov) / 2.0f);
            VKernel::Vector2 screen_center_uv =
                VKernel::Vector2(cursor_uv.x, 1 - cursor_uv.y) - VKernel::Vector2(0.5, 0.5);
            VKernel::Vector3 world_ray_dir = camera_forward * window_forward +
                                             camera_right * (float)game_engine_window_size.x * screen_center_uv.x +
                                             camera_up * (float)game_engine_window_size.y * screen_center_uv.y;

            // local ray -> tangent ray
            VKernel::Vector4 local_ray_origin = model_matrix.inverse() * VKernel::Vector4(camera_position, 1.0f);
            VKernel::Vector3 local_ray_origin_xyz =
                VKernel::Vector3(local_ray_origin.x, local_ray_origin.y, local_ray_origin.z);
            VKernel::Quaternion inversed_rotation = model_rotation.inverse();
            inversed_rotation.normalise();
            VKernel::Vector3 local_ray_dir = inversed_rotation * world_ray_dir;

            VKernel::Vector3 plane_normals[3] = {
                VKernel::Vector3(1, 0, 0), VKernel::Vector3(0, 1, 0), VKernel::Vector3(0, 0, 1)};

            float plane_view_depth[3] = {intersectPlaneRay(plane_normals[0], 0, local_ray_origin_xyz, local_ray_dir),
                                         intersectPlaneRay(plane_normals[1], 0, local_ray_origin_xyz, local_ray_dir),
                                         intersectPlaneRay(plane_normals[2], 0, local_ray_origin_xyz, local_ray_dir)};

            VKernel::Vector3 intersect_pt[3] = {local_ray_origin_xyz + plane_view_depth[0] * local_ray_dir,
                                                local_ray_origin_xyz + plane_view_depth[1] * local_ray_dir,
                                                local_ray_origin_xyz + plane_view_depth[2] * local_ray_dir};

            if ((int)m_axis_mode == 0 || (int)m_axis_mode == 2) ///< transition axis & scale axis
            {
                const float DIST_THRESHOLD   = 0.6f;
                const float EDGE_OF_AXIS_MIN = 0.1f;
                const float EDGE_OF_AXIS_MAX = 2.0f;
                const float AXIS_LENGTH      = 2.0f;

                float max_dist = 0.0f;
                for (int i = 0; i < 3; ++i) ///< Traverse three axes
                {
                    float local_ray_dir_proj = VKernel::Math::abs(local_ray_dir.dotProduct(plane_normals[i]));
                    float cos_alpha          = local_ray_dir_proj / 1.0f;
                    if (cos_alpha <= 0.15)
                    {
                        int   index00   = (i + 1) % 3;
                        int   index01   = 3 - i - index00;
                        int   index10   = (i + 2) % 3;
                        int   index11   = 3 - i - index10;
                        float axis_dist = (VKernel::Math::abs(intersect_pt[index00][i]) +
                                           VKernel::Math::abs(intersect_pt[index10][i])) /
                                          2;
                        if (axis_dist > DIST_THRESHOLD)
                        {
                            continue;
                        }
                        if ((intersect_pt[index00][index01] > EDGE_OF_AXIS_MIN) &&
                            (intersect_pt[index00][index01] < AXIS_LENGTH) &&
                            (intersect_pt[index00][index01] > max_dist) &&
                            (VKernel::Math::abs(intersect_pt[index00][i]) < EDGE_OF_AXIS_MAX))
                        {
                            max_dist        = intersect_pt[index00][index01];
                            m_selected_axis = index01;
                        }
                        if ((intersect_pt[index10][index11] > EDGE_OF_AXIS_MIN) &&
                            (intersect_pt[index10][index11] < AXIS_LENGTH) &&
                            (intersect_pt[index10][index11] > max_dist) &&
                            (VKernel::Math::abs(intersect_pt[index10][i]) < EDGE_OF_AXIS_MAX))
                        {
                            max_dist        = intersect_pt[index10][index11];
                            m_selected_axis = index11;
                        }
                    }
                }
                if (m_selected_axis == 3)
                {
                    float min_dist = 1e10f;
                    for (int i = 0; i < 3; ++i)
                    {
                        int   index0 = (i + 1) % 3;
                        int   index1 = (i + 2) % 3;
                        float dist   = VKernel::Math::sqr(intersect_pt[index0][index1]) +
                                     VKernel::Math::sqr(intersect_pt[index1][index0]);
                        if ((intersect_pt[index0][i] > EDGE_OF_AXIS_MIN) &&
                            (intersect_pt[index0][i] < EDGE_OF_AXIS_MAX) && (dist < DIST_THRESHOLD) &&
                            (dist < min_dist))
                        {
                            min_dist        = dist;
                            m_selected_axis = i;
                        }
                    }
                }
            }
            else if ((int)m_axis_mode == 1) ///< rotation axis
            {
                const float DIST_THRESHOLD = 0.2f;

                float min_dist = 1e10f;
                for (int i = 0; i < 3; ++i)
                {
                    const float dist =
                        std::fabs(1 - std::hypot(intersect_pt[i].x, intersect_pt[i].y, intersect_pt[i].z));
                    if ((dist < DIST_THRESHOLD) && (dist < min_dist))
                    {
                        min_dist        = dist;
                        m_selected_axis = i;
                    }
                }
            }
            else
            {
                return m_selected_axis;
            }
        }

        g_editor_global_context.m_render_system->setSelectedAxis(m_selected_axis);

        return m_selected_axis;
    }
} // namespace ReCoder

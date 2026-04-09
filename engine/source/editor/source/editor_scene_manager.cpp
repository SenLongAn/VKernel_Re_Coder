#include "editor/include/editor_scene_manager.h"

#include "editor/include/editor_global_context.h"
#include "runtime/engine.h"
#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"
#include "runtime/core/base/macro.h"

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

    std::pair<uint32_t, VKernel::Vector4>
    EditorSceneManager::getGuidOfPickedMesh(const VKernel::Vector2 &picked_uv) const
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

    VKernel::RenderEntity *EditorSceneManager::getAxisMeshByType(EditorAxisMode axis_mode)
    {
        VKernel::RenderEntity *axis_mesh = nullptr; ///< base class
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
        bool isUpdate = false;
        if (m_selected_gobject_id != selected_gobject_id)
            isUpdate = true;
        m_selected_gobject_id = selected_gobject_id;
        VKernel::g_runtime_global_context.m_render_system->setGOId(m_selected_gobject_id);

        // get GO
        std::shared_ptr<VKernel::GObject> selected_gobject = getSelectedGObject().lock();
        if (selected_gobject)
        {
            if (isUpdate)
                LOG_INFO(selected_gobject->getName());
            VKernel::g_runtime_global_context.m_render_system->setGo(selected_gobject);
            // get TransformComponent matrix
            VKernel::TransformComponent *transform_component =
                selected_gobject->tryGetComponent(VKernel::TransformComponent, "TransformComponent");
            m_selected_object_matrix = transform_component->getMatrix();
        }

        // update axis Model matrix
        drawSelectedEntityAxis();
    }

    void EditorSceneManager::uploadAxisResource()
    {
        // get guid allocator
        auto &instance_id_allocator = g_editor_global_context.m_render_system->getGOInstanceIdAllocator();
        auto &mesh_asset_id_allocator = g_editor_global_context.m_render_system->getMeshAssetIdAllocator();

        // assign some value that won't be used by other game objects
        {
            VKernel::GameObjectPartId axis_instance_id = {0xFFAA, 0xFFAA};
            VKernel::MeshSourceDesc mesh_source_desc = {"%%translation_axis%%"};

            m_translation_axis.m_instance_id = instance_id_allocator.allocGuid(axis_instance_id);
            m_translation_axis.m_mesh_asset_id = mesh_asset_id_allocator.allocGuid(mesh_source_desc);
        }

        {
            VKernel::GameObjectPartId axis_instance_id = {0xFFBB, 0xFFBB};
            VKernel::MeshSourceDesc mesh_source_desc = {"%%rotate_axis%%"};

            m_rotation_axis.m_instance_id = instance_id_allocator.allocGuid(axis_instance_id);
            m_rotation_axis.m_mesh_asset_id = mesh_asset_id_allocator.allocGuid(mesh_source_desc);
        }

        {
            VKernel::GameObjectPartId axis_instance_id = {0xFFCC, 0xFFCC};
            VKernel::MeshSourceDesc mesh_source_desc = {"%%scale_axis%%"};

            m_scale_aixs.m_instance_id = instance_id_allocator.allocGuid(axis_instance_id);
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

        if (VKernel::g_is_editor_mode && selected_object != nullptr)
        {
            // get selected object TransformComponent
            VKernel::TransformComponent *transform_component =
                selected_object->tryGetComponent(VKernel::TransformComponent, "TransformComponent");

            // decomposition
            VKernel::Vector3 scale;
            VKernel::Quaternion rotation;
            VKernel::Vector3 translation;
            transform_component->getMatrix().decomposition(translation, scale, rotation);

            // The model matrix of the axis
            VKernel::Matrix4x4 translation_matrix = VKernel::Matrix4x4::getTrans(translation);
            VKernel::Matrix4x4 scale_matrix = VKernel::Matrix4x4::buildScaleMatrix(1.0f, 1.0f, 1.0f);
            VKernel::Matrix4x4 axis_model_matrix = translation_matrix * scale_matrix;

            VKernel::RenderEntity *selected_aixs = getAxisMeshByType(m_axis_mode); ///< get axis object

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
        float camera_fov = m_camera->getFovYDeprecated();
        VKernel::Vector3 camera_forward = m_camera->forward();
        VKernel::Vector3 camera_up = m_camera->up();
        VKernel::Vector3 camera_right = m_camera->right();
        VKernel::Vector3 camera_position = m_camera->position();

        // Is the object selected
        if (m_selected_gobject_id == VKernel::k_invalid_gobject_id)
        {
            return m_selected_axis;
        }

        // get axis object
        VKernel::RenderEntity *selected_aixs = getAxisMeshByType(m_axis_mode);

        m_selected_axis = 3;
        if (m_is_show_axis == false)
        {
            return m_selected_axis;
        }
        else
        {
            // decomposition
            VKernel::Matrix4x4 model_matrix = selected_aixs->m_model_matrix;
            VKernel::Vector3 model_scale;
            VKernel::Quaternion model_rotation;
            VKernel::Vector3 model_translation;
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
                const float DIST_THRESHOLD = 0.6f;
                const float EDGE_OF_AXIS_MIN = 0.1f;
                const float EDGE_OF_AXIS_MAX = 2.0f;
                const float AXIS_LENGTH = 2.0f;

                float max_dist = 0.0f;
                for (int i = 0; i < 3; ++i) ///< Traverse three axes
                {
                    float local_ray_dir_proj = VKernel::Math::abs(local_ray_dir.dotProduct(plane_normals[i]));
                    float cos_alpha = local_ray_dir_proj / 1.0f;
                    if (cos_alpha <= 0.15)
                    {
                        int index00 = (i + 1) % 3;
                        int index01 = 3 - i - index00;
                        int index10 = (i + 2) % 3;
                        int index11 = 3 - i - index10;
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
                            max_dist = intersect_pt[index00][index01];
                            m_selected_axis = index01;
                        }
                        if ((intersect_pt[index10][index11] > EDGE_OF_AXIS_MIN) &&
                            (intersect_pt[index10][index11] < AXIS_LENGTH) &&
                            (intersect_pt[index10][index11] > max_dist) &&
                            (VKernel::Math::abs(intersect_pt[index10][i]) < EDGE_OF_AXIS_MAX))
                        {
                            max_dist = intersect_pt[index10][index11];
                            m_selected_axis = index11;
                        }
                    }
                }
                if (m_selected_axis == 3)
                {
                    float min_dist = 1e10f;
                    for (int i = 0; i < 3; ++i)
                    {
                        int index0 = (i + 1) % 3;
                        int index1 = (i + 2) % 3;
                        float dist = VKernel::Math::sqr(intersect_pt[index0][index1]) +
                                     VKernel::Math::sqr(intersect_pt[index1][index0]);
                        if ((intersect_pt[index0][i] > EDGE_OF_AXIS_MIN) &&
                            (intersect_pt[index0][i] < EDGE_OF_AXIS_MAX) && (dist < DIST_THRESHOLD) &&
                            (dist < min_dist))
                        {
                            min_dist = dist;
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
                        min_dist = dist;
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

    void ReCoder::EditorSceneManager::moveEntity(float new_mouse_pos_x,
                                                 float new_mouse_pos_y,
                                                 float last_mouse_pos_x,
                                                 float last_mouse_pos_y,
                                                 VKernel::Vector2 engine_window_pos,
                                                 VKernel::Vector2 engine_window_size,
                                                 size_t cursor_on_axis,
                                                 VKernel::Matrix4x4 model_matrix)
    {
        // get selected object
        std::shared_ptr<VKernel::GObject> selected_object = getSelectedGObject().lock();
        if (selected_object == nullptr)
            return;

        // calculate volocity and mouse delta
        float angularVelocity = 18.0f / VKernel::Math::max(engine_window_size.x, engine_window_size.y);
        VKernel::Vector2 delta_mouse_move_uv = {(new_mouse_pos_x - last_mouse_pos_x),
                                                (new_mouse_pos_y - last_mouse_pos_y)};

        // decomposition
        VKernel::Vector3 model_scale;
        VKernel::Quaternion model_rotation;
        VKernel::Vector3 model_translation;
        model_matrix.decomposition(model_translation, model_scale, model_rotation);

        // Vector3 -> matrix
        VKernel::Matrix4x4 axis_model_matrix = VKernel::Matrix4x4::IDENTITY;
        axis_model_matrix.setTrans(model_translation);

        // GO origin -> screen uv
        VKernel::Matrix4x4 view_matrix = m_camera->getLookAtMatrix();
        VKernel::Matrix4x4 proj_matrix = m_camera->getPersProjMatrix();
        VKernel::Vector4 model_world_position_4(model_translation, 1.f);
        VKernel::Vector4 model_origin_clip_position = proj_matrix * view_matrix * model_world_position_4; ///< MVP
        model_origin_clip_position /= model_origin_clip_position.w;                                       ///< /w
        VKernel::Vector2 model_origin_clip_uv =
            VKernel::Vector2((model_origin_clip_position.x + 1) / 2.0f, (model_origin_clip_position.y + 1) / 2.0f);

        // xyz gizmo axis -> screen vec2
        VKernel::Vector4 axis_x_local_position_4(1, 0, 0, 1);
        if (m_axis_mode == EditorAxisMode::ScaleMode)
        {
            axis_x_local_position_4 = VKernel::Matrix4x4(model_rotation) * axis_x_local_position_4;
        }
        VKernel::Vector4 axis_x_world_position_4 = axis_model_matrix * axis_x_local_position_4;
        axis_x_world_position_4.w = 1.0f;
        VKernel::Vector4 axis_x_clip_position = proj_matrix * view_matrix * axis_x_world_position_4;
        axis_x_clip_position /= axis_x_clip_position.w;
        VKernel::Vector2 axis_x_clip_uv((axis_x_clip_position.x + 1) / 2.0f, (axis_x_clip_position.y + 1) / 2.0f);
        VKernel::Vector2 axis_x_direction_uv = axis_x_clip_uv - model_origin_clip_uv;
        axis_x_direction_uv.normalise();

        VKernel::Vector4 axis_y_local_position_4(0, 1, 0, 1);
        if (m_axis_mode == EditorAxisMode::ScaleMode)
        {
            axis_y_local_position_4 = VKernel::Matrix4x4(model_rotation) * axis_y_local_position_4;
        }
        VKernel::Vector4 axis_y_world_position_4 = axis_model_matrix * axis_y_local_position_4;
        axis_y_world_position_4.w = 1.0f;
        VKernel::Vector4 axis_y_clip_position = proj_matrix * view_matrix * axis_y_world_position_4;
        axis_y_clip_position /= axis_y_clip_position.w;
        VKernel::Vector2 axis_y_clip_uv((axis_y_clip_position.x + 1) / 2.0f, (axis_y_clip_position.y + 1) / 2.0f);
        VKernel::Vector2 axis_y_direction_uv = axis_y_clip_uv - model_origin_clip_uv;
        axis_y_direction_uv.normalise();

        VKernel::Vector4 axis_z_local_position_4(0, 0, 1, 1);
        if (m_axis_mode == EditorAxisMode::ScaleMode)
        {
            axis_z_local_position_4 = VKernel::Matrix4x4(model_rotation) * axis_z_local_position_4;
        }
        VKernel::Vector4 axis_z_world_position_4 = axis_model_matrix * axis_z_local_position_4;
        axis_z_world_position_4.w = 1.0f;
        VKernel::Vector4 axis_z_clip_position = proj_matrix * view_matrix * axis_z_world_position_4;
        axis_z_clip_position /= axis_z_clip_position.w;
        VKernel::Vector2 axis_z_clip_uv((axis_z_clip_position.x + 1) / 2.0f, (axis_z_clip_position.y + 1) / 2.0f);
        VKernel::Vector2 axis_z_direction_uv = axis_z_clip_uv - model_origin_clip_uv;
        axis_z_direction_uv.normalise();

        // calculate and update
        VKernel::TransformComponent *transform_component =
            selected_object->tryGetComponent(VKernel::TransformComponent, "TransformComponent");
        VKernel::Matrix4x4 new_model_matrix(VKernel::Matrix4x4::IDENTITY);
        if (m_axis_mode == EditorAxisMode::TranslateMode) // translate
        {
            // calculate move vector
            VKernel::Vector3 move_vector = {0, 0, 0};
            if (cursor_on_axis == 0)
            {
                move_vector.x =
                    delta_mouse_move_uv.dotProduct(axis_x_direction_uv) * angularVelocity; ///< delta, dir, velocity
            }
            else if (cursor_on_axis == 1)
            {
                move_vector.y = delta_mouse_move_uv.dotProduct(axis_y_direction_uv) * angularVelocity;
            }
            else if (cursor_on_axis == 2)
            {
                move_vector.z = delta_mouse_move_uv.dotProduct(axis_z_direction_uv) * angularVelocity;
            }
            else
            {
                return;
            }
            VKernel::Matrix4x4 translate_mat;
            translate_mat.makeTransform(move_vector, VKernel::Vector3::UNIT_SCALE, VKernel::Quaternion::IDENTITY);
            new_model_matrix = axis_model_matrix * translate_mat;

            // calculate new M matrix
            new_model_matrix = new_model_matrix * VKernel::Matrix4x4(model_rotation);
            new_model_matrix =
                new_model_matrix * VKernel::Matrix4x4::buildScaleMatrix(model_scale.x, model_scale.y, model_scale.z);
            VKernel::Vector3 new_scale;
            VKernel::Quaternion new_rotation;
            VKernel::Vector3 new_translation;
            new_model_matrix.decomposition(
                new_translation, new_scale, new_rotation); ///< Invariant under rotation and scaling

            // update GO transform
            VKernel::Matrix4x4 translation_matrix = VKernel::Matrix4x4::getTrans(new_translation);
            VKernel::Matrix4x4 scale_matrix = VKernel::Matrix4x4::buildScaleMatrix(1.f, 1.f, 1.f);
            VKernel::Matrix4x4 axis_model_matrix = translation_matrix * scale_matrix;

            transform_component->setPosition(new_translation);
            transform_component->setRotation(new_rotation);
            transform_component->setScale(new_scale);

            // update axis M matrix
            m_translation_axis.m_model_matrix = axis_model_matrix;
            m_rotation_axis.m_model_matrix = axis_model_matrix;
            m_scale_aixs.m_model_matrix = axis_model_matrix;

            g_editor_global_context.m_render_system->setVisibleAxis(m_translation_axis);
        }
        else if (m_axis_mode == EditorAxisMode::RotateMode) // rotate
        {
            // calculate move
            float last_mouse_u = (last_mouse_pos_x - engine_window_pos.x) / engine_window_size.x;
            float last_mouse_v = (last_mouse_pos_y - engine_window_pos.y) / engine_window_size.y;
            VKernel::Vector2 last_move_vector(last_mouse_u - model_origin_clip_uv.x,
                                              last_mouse_v - model_origin_clip_uv.y);
            float new_mouse_u = (new_mouse_pos_x - engine_window_pos.x) / engine_window_size.x;
            float new_mouse_v = (new_mouse_pos_y - engine_window_pos.y) / engine_window_size.y;
            VKernel::Vector2 new_move_vector(new_mouse_u - model_origin_clip_uv.x,
                                             new_mouse_v - model_origin_clip_uv.y);
            VKernel::Vector3 delta_mouse_uv_3(delta_mouse_move_uv.x, delta_mouse_move_uv.y, 0);
            float move_radian;
            VKernel::Vector3 axis_of_rotation = {0, 0, 0};
            if (cursor_on_axis == 0)
            {
                move_radian = (delta_mouse_move_uv * angularVelocity).length();
                if (m_camera->forward().dotProduct(VKernel::Vector3::UNIT_X) < 0)
                {
                    move_radian = -move_radian;
                }
                axis_of_rotation.x = 1;
            }
            else if (cursor_on_axis == 1)
            {
                move_radian = (delta_mouse_move_uv * angularVelocity).length(); ///< move radian
                if (m_camera->forward().dotProduct(VKernel::Vector3::UNIT_Y) < 0)
                {
                    move_radian = -move_radian;
                }
                axis_of_rotation.y = 1;
            }
            else if (cursor_on_axis == 2)
            {
                move_radian = (delta_mouse_move_uv * angularVelocity).length();
                if (m_camera->forward().dotProduct(VKernel::Vector3::UNIT_Z) < 0)
                {
                    move_radian = -move_radian;
                }
                axis_of_rotation.z = 1;
            }
            else
            {
                return;
            }
            float move_direction =
                last_move_vector.x * new_move_vector.y - new_move_vector.x * last_move_vector.y; ///< move direction
            if (move_direction < 0)
            {
                move_radian = -move_radian;
            }

            // calculate new M matrix
            VKernel::Quaternion move_rot;
            move_rot.fromAngleAxis(VKernel::Radian(move_radian), axis_of_rotation);
            new_model_matrix = axis_model_matrix * move_rot;
            new_model_matrix = new_model_matrix * VKernel::Matrix4x4(model_rotation);
            new_model_matrix =
                new_model_matrix * VKernel::Matrix4x4::buildScaleMatrix(model_scale.x, model_scale.y, model_scale.z);

            VKernel::Vector3 new_scale;
            VKernel::Quaternion new_rotation;
            VKernel::Vector3 new_translation;
            new_model_matrix.decomposition(new_translation, new_scale, new_rotation);

            if (!transform_component)
            {
                return;
            }
            // update GO transform
            transform_component->setPosition(new_translation);
            transform_component->setRotation(new_rotation);
            transform_component->setScale(new_scale);

            // update axis M matrix
            m_scale_aixs.m_model_matrix = new_model_matrix;
        }
        else if (m_axis_mode == EditorAxisMode::ScaleMode) // scale
        {
            VKernel::Vector3 delta_scale_vector = {0, 0, 0};
            VKernel::Vector3 new_model_scale = {0, 0, 0};
            if (cursor_on_axis == 0)
            {
                delta_scale_vector.x = 0.01f;
                if (delta_mouse_move_uv.dotProduct(axis_x_direction_uv) < 0)
                {
                    delta_scale_vector = -delta_scale_vector;
                }
            }
            else if (cursor_on_axis == 1)
            {
                delta_scale_vector.y = 0.01f;
                if (delta_mouse_move_uv.dotProduct(axis_y_direction_uv) < 0)
                {
                    delta_scale_vector = -delta_scale_vector;
                }
            }
            else if (cursor_on_axis == 2)
            {
                delta_scale_vector.z = 0.01f;
                if (delta_mouse_move_uv.dotProduct(axis_z_direction_uv) < 0)
                {
                    delta_scale_vector = -delta_scale_vector;
                }
            }
            else
            {
                return;
            }
            new_model_scale = model_scale + delta_scale_vector;
            axis_model_matrix = axis_model_matrix * VKernel::Matrix4x4(model_rotation);
            VKernel::Matrix4x4 scale_mat;
            scale_mat.makeTransform(VKernel::Vector3::ZERO, new_model_scale, VKernel::Quaternion::IDENTITY);
            new_model_matrix = axis_model_matrix * scale_mat;
            VKernel::Vector3 new_scale;
            VKernel::Quaternion new_rotation;
            VKernel::Vector3 new_translation;
            new_model_matrix.decomposition(new_translation, new_scale, new_rotation);

            transform_component->setPosition(new_translation);
            transform_component->setRotation(new_rotation);
            transform_component->setScale(new_scale);
        }

        // update Selected Object Matrix
        setSelectedObjectMatrix(new_model_matrix);
    }
} // namespace ReCoder

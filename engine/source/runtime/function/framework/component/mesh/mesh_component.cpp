#include "runtime/function/framework/component/mesh/mesh_component.h"

#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/res_type/data/material.h"

#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/object/object.h"
#include "runtime/function/global/global_context.h"

#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_system.h"

namespace VKernel
{
    void MeshComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        // Writing data to m_raw_meshes

        m_parent_object = parent_object;

        std::shared_ptr<AssetManager> asset_manager = g_runtime_global_context.m_asset_manager;

        m_raw_meshes.resize(m_mesh_res.m_sub_meshes.size()); ///< resize

        size_t raw_mesh_count = 0;
        for (SubMeshRes& sub_mesh : m_mesh_res.m_sub_meshes) ///< Traverse all subgrids
        {
            GameObjectPartDesc& meshComponent = m_raw_meshes[raw_mesh_count]; ///< current submesh

            meshComponent.m_mesh_desc.m_mesh_file =
                asset_manager->getFullPath(sub_mesh.m_obj_file_ref).generic_string(); ///< full path

            meshComponent.m_material_desc.m_with_texture =
                sub_mesh.m_material.empty() == false; ///< Does the material file path exist?

            if (meshComponent.m_material_desc.m_with_texture) ///< if exist
            {
                // write material file path
                MaterialRes material_res;
                asset_manager->loadAsset(sub_mesh.m_material, material_res);

                meshComponent.m_material_desc.m_base_color_texture_file =
                    asset_manager->getFullPath(material_res.m_base_colour_texture_file).generic_string();
                meshComponent.m_material_desc.m_normal_texture_file =
                    asset_manager->getFullPath(material_res.m_normal_texture_file).generic_string();
            }

            // write transform
            auto object_space_transform                       = sub_mesh.m_transform.getMatrix();
            meshComponent.m_transform_desc.m_transform_matrix = object_space_transform;
            meshComponent.m_color                             = m_mesh_res.m_color;
            if (m_mesh_res.m_apply_lighting)
            {
                meshComponent.m_apply_lighting = Vector3::UNIT_SCALE;
            }
            else
            {
                meshComponent.m_apply_lighting = Vector3::ZERO;
            }
            if (m_mesh_res.m_apply_texture)
            {
                meshComponent.m_apply_texture = 1;
            }
            else
            {
                meshComponent.m_apply_texture = 0;
            }

            // add count
            ++raw_mesh_count;
        }
    }

    void MeshComponent::tick(float delta_time)
    {
        if (!m_parent_object.lock())
            return;

        // Get the transformation of the associated object
        TransformComponent* transform_component =
            m_parent_object.lock()->tryGetComponent(TransformComponent, "TransformComponent");

        std::vector<GameObjectPartDesc> dirty_mesh_parts;
        for (GameObjectPartDesc& mesh_part : m_raw_meshes) ///< Traverse all the grids of this object
        {
            // Compute Subgrid Transform
            Matrix4x4 object_transform_matrix = mesh_part.m_transform_desc.m_transform_matrix;
            mesh_part.m_transform_desc.m_transform_matrix =
                transform_component->getMatrix() *
                object_transform_matrix; ///< Parent Object Transform * Submesh Relative Transform
            mesh_part.m_color = m_mesh_res.m_color;
            if (m_mesh_res.m_apply_lighting)
            {
                mesh_part.m_apply_lighting = Vector3::UNIT_SCALE;
            }
            else
            {
                mesh_part.m_apply_lighting = Vector3::ZERO;
            }
            if (m_mesh_res.m_apply_texture)
            {
                mesh_part.m_apply_texture = 1;
            }
            else
            {
                mesh_part.m_apply_texture = 0;
            }

            dirty_mesh_parts.push_back(mesh_part); ///< add

            mesh_part.m_transform_desc.m_transform_matrix = object_transform_matrix; ///< Restore
        }

        // Add to swap Context
        RenderSwapContext& render_swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        RenderSwapData&    logic_swap_data     = render_swap_context.getLogicSwapData();
        logic_swap_data.addDirtyGameObject(GameObjectDesc(m_parent_object.lock()->getID(), dirty_mesh_parts));
    }
} // namespace VKernel

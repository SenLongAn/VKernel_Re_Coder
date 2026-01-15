#include "runtime/function/render/render_scene.h"

#include "render_scene.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_helper.h"
#include "runtime/function/render/render_pass.h"
#include "runtime/function/render/render_resource.h"

namespace VKernel
{
    void RenderScene::updateVisibleObjects(std::shared_ptr<RenderResource> render_resource,
                                           std::shared_ptr<RenderCamera>   camera)
    {
        updateVisibleObjectsDirectionalLight(render_resource, camera);
        updateVisibleObjectsPointLight(render_resource);
        updateVisibleObjectsMainCamera(render_resource, camera);
        updateVisibleObjectsAxis(render_resource);
    }

    void RenderScene::setVisibleNodesReference()
    {
        RenderPass::m_visiable_nodes.p_directional_light_visible_mesh_nodes = &m_directional_light_visible_mesh_nodes;
        RenderPass::m_visiable_nodes.p_point_lights_visible_mesh_nodes      = &m_point_lights_visible_mesh_nodes;
        RenderPass::m_visiable_nodes.p_main_camera_visible_mesh_nodes       = &m_main_camera_visible_mesh_nodes;
        RenderPass::m_visiable_nodes.p_axis_node                            = &m_axis_node;
    }

    void RenderScene::clearForLevelReloading() { m_render_entities.clear(); }

    GuidAllocator<GameObjectPartId>& RenderScene::getInstanceIdAllocator() { return m_instance_id_allocator; }

    GuidAllocator<MeshSourceDesc>& RenderScene::getMeshAssetIdAllocator() { return m_mesh_asset_id_allocator; }

    GuidAllocator<MaterialSourceDesc>& RenderScene::getMaterialAssetdAllocator()
    {
        return m_material_asset_id_allocator;
    }

    void RenderScene::addInstanceIdToMap(uint32_t instance_id, GObjectID go_id)
    {
        m_mesh_object_id_map[instance_id] = go_id;
    }

    GObjectID RenderScene::getGObjectIDByMeshID(uint32_t mesh_id) const
    {
        auto find_it = m_mesh_object_id_map.find(mesh_id);
        if (find_it != m_mesh_object_id_map.end())
        {
            return find_it->second;
        }
        return GObjectID();
    }

    void RenderScene::updateVisibleObjectsDirectionalLight(std::shared_ptr<RenderResource> render_resource,
                                                           std::shared_ptr<RenderCamera>   camera)
    {
        // light PV
        Matrix4x4 directional_light_proj_view = CalculateDirectionalLightCamera(*this, *camera);

        // Set to render_resource
        render_resource->m_mesh_perframe_storage_buffer_object.directional_light_proj_view =
            directional_light_proj_view;
        render_resource->m_mesh_directional_light_shadow_perframe_storage_buffer_object.light_proj_view =
            directional_light_proj_view;

        m_directional_light_visible_mesh_nodes.clear();

        // TODO: Frustum
        // Iterative entity
        for (const RenderEntity& entity : m_render_entities)
        {
            // add null node
            m_directional_light_visible_mesh_nodes.emplace_back();
            RenderMeshNode& temp_node = m_directional_light_visible_mesh_nodes.back();

            // set node
            temp_node.node_id                 = entity.m_instance_id;
            temp_node.model_matrix            = &entity.m_model_matrix;
            VulkanMesh& mesh_asset            = render_resource->getEntityMesh(entity);
            temp_node.ref_mesh                = &mesh_asset;
            VulkanPBRMaterial& material_asset = render_resource->getEntityMaterial(entity);
            temp_node.ref_material            = &material_asset;
        }
    }

    void RenderScene::updateVisibleObjectsPointLight(std::shared_ptr<RenderResource> render_resource)
    {
        m_point_lights_visible_mesh_nodes.clear();

        // TODO: Frustum

        // Iterative entity
        for (const RenderEntity& entity : m_render_entities)
        {
            // add null node
            m_point_lights_visible_mesh_nodes.emplace_back();
            RenderMeshNode& temp_node = m_point_lights_visible_mesh_nodes.back();

            // set node
            temp_node.node_id                 = entity.m_instance_id;
            temp_node.model_matrix            = &entity.m_model_matrix;
            VulkanMesh& mesh_asset            = render_resource->getEntityMesh(entity);
            temp_node.ref_mesh                = &mesh_asset;
            VulkanPBRMaterial& material_asset = render_resource->getEntityMaterial(entity);
            temp_node.ref_material            = &material_asset;
        }
    }

    void RenderScene::updateVisibleObjectsMainCamera(std::shared_ptr<RenderResource> render_resource,
                                                     std::shared_ptr<RenderCamera>   camera)
    {
        // TODO: Frustum
        // clear
        m_main_camera_visible_mesh_nodes.clear();

        // Iterative entity
        for (const auto& entity : m_render_entities)
        {
            // add null node
            m_main_camera_visible_mesh_nodes.emplace_back();
            RenderMeshNode& temp_node = m_main_camera_visible_mesh_nodes.back();

            // set node
            temp_node.node_id                 = entity.m_instance_id;
            temp_node.model_matrix            = &entity.m_model_matrix;                 ///< model matrix
            VulkanMesh& mesh_asset            = render_resource->getEntityMesh(entity); ///< mesh
            temp_node.ref_mesh                = &mesh_asset;
            VulkanPBRMaterial& material_asset = render_resource->getEntityMaterial(entity); ///< material
            temp_node.ref_material            = &material_asset;
        }
    }
    void RenderScene::updateVisibleObjectsAxis(std::shared_ptr<RenderResource> render_resource)
    {
        if (m_render_axis.has_value())
        {
            RenderEntity& axis = *m_render_axis;

            m_axis_node.model_matrix = axis.m_model_matrix;
            m_axis_node.node_id      = axis.m_instance_id;

            VulkanMesh& mesh_asset = render_resource->getEntityMesh(axis);
            m_axis_node.ref_mesh   = &mesh_asset;
        }
    }
} // namespace VKernel

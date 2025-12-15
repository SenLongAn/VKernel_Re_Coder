#include "runtime/function/render/render_scene.h"

#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_pass.h"
#include "runtime/function/render/render_resource.h"

namespace VKernel
{
    void RenderScene::updateVisibleObjects(std::shared_ptr<RenderResource> render_resource,
                                           std::shared_ptr<RenderCamera>   camera)
    {
        updateVisibleObjectsMainCamera(render_resource, camera);
    }

    void RenderScene::setVisibleNodesReference()
    {
        RenderPass::m_visiable_nodes.p_main_camera_visible_mesh_nodes = &m_main_camera_visible_mesh_nodes;
    }

    void RenderScene::updateVisibleObjectsMainCamera(std::shared_ptr<RenderResource> render_resource,
                                                     std::shared_ptr<RenderCamera>   camera)
    {
        // clear
        m_main_camera_visible_mesh_nodes.clear();

        // Iterative mesh
        for (const auto& entity : m_render_entities)
        {
            // add null node
            m_main_camera_visible_mesh_nodes.emplace_back();
            RenderMeshNode& temp_node = m_main_camera_visible_mesh_nodes.back();

            // set node
            temp_node.model_matrix = &entity.m_model_matrix;                 ///< model matrix
            VulkanMesh& mesh_asset = render_resource->getEntityMesh(entity); ///< mesh
            temp_node.ref_mesh     = &mesh_asset;
        }
    }
} // namespace VKernel

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

        // proj_view_matrix
        Matrix4x4 view_matrix      = camera->getViewMatrix();
        Matrix4x4 proj_matrix      = camera->getPersProjMatrix();
        Matrix4x4 proj_view_matrix = proj_matrix * view_matrix;

        // Iterative mesh
        for (const auto& entity : render_resource->m_vulkan_meshes)
        {
            // add null node
            m_main_camera_visible_mesh_nodes.emplace_back();
            RenderMeshNode& temp_node = m_main_camera_visible_mesh_nodes.back();

            // set node
            VulkanMesh& mesh_asset = render_resource->getEntityMesh(entity.first);
            temp_node.ref_mesh     = &mesh_asset;
        }
    }
} // namespace VKernel

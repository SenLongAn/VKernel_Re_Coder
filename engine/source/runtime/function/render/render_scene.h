#pragma once

#include "runtime/function/render/light.h"
#include "runtime/function/render/render_common.h"
#include "runtime/function/render/render_entity.h"

#include <memory>
#include <vector>

/**
 * Manage Scenario GO Data
 */
namespace VKernel
{
    class RenderResource;
    class RenderCamera;

    class RenderScene
    {
    public:
        // light
        AmbientLight      m_ambient_light;
        PDirectionalLight m_directional_light;
        PointLightList    m_point_light_list;

        // visible objects
        std::vector<RenderMeshNode> m_directional_light_visible_mesh_nodes;
        std::vector<RenderMeshNode> m_main_camera_visible_mesh_nodes;

        // render entities
        std::vector<RenderEntity> m_render_entities;

    public:
        // update visible objects in each frame
        void updateVisibleObjects(std::shared_ptr<RenderResource> render_resource,
                                  std::shared_ptr<RenderCamera>   camera);

        // set visible nodes ptr in render pass
        void setVisibleNodesReference();

    private:
        // update Visible Object
        void updateVisibleObjectsDirectionalLight(std::shared_ptr<RenderResource> render_resource,
                                                  std::shared_ptr<RenderCamera>   camera);
        void updateVisibleObjectsMainCamera(std::shared_ptr<RenderResource> render_resource,
                                            std::shared_ptr<RenderCamera>   camera);
    };
} // namespace VKernel
#pragma once

#include "runtime/function/render/render_common.h"

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
        // visible objects
        std::vector<RenderMeshNode> m_main_camera_visible_mesh_nodes;

    public:
        // update visible objects in each frame
        void updateVisibleObjects(std::shared_ptr<RenderResource> render_resource,
                                  std::shared_ptr<RenderCamera>   camera);

        // set visible nodes ptr in render pass
        void setVisibleNodesReference();

    private:
        void updateVisibleObjectsMainCamera(std::shared_ptr<RenderResource> render_resource,
                                            std::shared_ptr<RenderCamera>   camera);
    };
} // namespace VKernel
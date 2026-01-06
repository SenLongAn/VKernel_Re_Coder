#pragma once

#include "runtime/function/render/light.h"
#include "runtime/function/render/render_common.h"
#include "runtime/function/render/render_entity.h"
#include "runtime/function/render/render_guid_allocator.h"
#include "runtime/function/render/render_object.h"

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

        void clearForLevelReloading(); ///< clear level

        // get guid allocator
        GuidAllocator<GameObjectPartId>&   getInstanceIdAllocator();
        GuidAllocator<MeshSourceDesc>&     getMeshAssetIdAllocator();
        GuidAllocator<MaterialSourceDesc>& getMaterialAssetdAllocator();

        void      addInstanceIdToMap(uint32_t instance_id, GObjectID go_id); ///< add guid to m_mesh_object_id_map
        GObjectID getGObjectIDByMeshID(uint32_t mesh_id) const;              ///< get

    private:
        GuidAllocator<GameObjectPartId>   m_instance_id_allocator;       ///< submesh guid allocator
        GuidAllocator<MeshSourceDesc>     m_mesh_asset_id_allocator;     ///< mesh guid allocator
        GuidAllocator<MaterialSourceDesc> m_material_asset_id_allocator; ///< material guid allocator

        std::unordered_map<uint32_t, GObjectID> m_mesh_object_id_map; // submesh guid, object guid

    private:
        // update Visible Object
        void updateVisibleObjectsDirectionalLight(std::shared_ptr<RenderResource> render_resource,
                                                  std::shared_ptr<RenderCamera>   camera);
        void updateVisibleObjectsMainCamera(std::shared_ptr<RenderResource> render_resource,
                                            std::shared_ptr<RenderCamera>   camera);
    };
} // namespace VKernel
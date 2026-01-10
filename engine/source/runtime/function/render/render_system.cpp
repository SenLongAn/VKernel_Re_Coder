#include "runtime/function/render/render_system.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/debugdraw/debug_draw_manager.h"
#include "runtime/function/render/passes/main_camera_pass.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_pass.h"
#include "runtime/function/render/render_pipeline.h"
#include "runtime/function/render/render_resource.h"
#include "runtime/function/render/render_resource_base.h"
#include "runtime/function/render/render_scene.h"
#include "runtime/function/render/vulkan_interface/vulkan_api.h"

#include "render_system.h"
#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/config_manager/config_manager.h"

namespace VKernel
{

    RenderSystem::~RenderSystem() { clear(); }

    void RenderSystem::initialize(std::shared_ptr<WindowSystem> window_system)
    {
        // init
        std::shared_ptr<ConfigManager> config_manager = g_runtime_global_context.m_config_manager;
        std::shared_ptr<AssetManager>  asset_manager  = g_runtime_global_context.m_asset_manager;

        // vulkan api
        m_vulkan_api = std::make_shared<VulkanAPI>();
        m_vulkan_api->initialize(window_system);
        // global rendering resource
        GlobalRenderingRes global_rendering_res;
        const std::string& global_rendering_res_url = config_manager->getGlobalRenderingResUrl();
        asset_manager->loadAsset(global_rendering_res_url, global_rendering_res);

        // resource
        LevelResourceDesc level_resource_desc;
        level_resource_desc.m_ibl_resource_desc.m_skybox_irradiance_map = global_rendering_res.m_skybox_irradiance_map;
        level_resource_desc.m_ibl_resource_desc.m_skybox_specular_map   = global_rendering_res.m_skybox_specular_map;
        level_resource_desc.m_ibl_resource_desc.m_brdf_map              = global_rendering_res.m_brdf_map;

        m_render_resource = std::make_shared<RenderResource>();
        m_render_resource->uploadGlobalRenderResource(m_vulkan_api, level_resource_desc);

        // camera
        const CameraPose& camera_pose = global_rendering_res.m_camera_config.m_pose;
        m_render_camera               = std::make_shared<RenderCamera>();
        m_render_camera->lookAt(camera_pose.m_position, camera_pose.m_target, camera_pose.m_up);
        m_render_camera->setZFar(global_rendering_res.m_camera_config.m_z_far);
        m_render_camera->setZNear(global_rendering_res.m_camera_config.m_z_near);
        m_render_camera->setAspect(global_rendering_res.m_camera_config.m_aspect.x /
                                   global_rendering_res.m_camera_config.m_aspect.y);

        // setup render scene
        m_render_scene = std::make_shared<RenderScene>();
        m_render_scene->setVisibleNodesReference();
        m_render_scene->m_directional_light.m_direction =
            global_rendering_res.m_directional_light.m_direction.normalisedCopy();
        m_render_scene->m_directional_light.m_color  = global_rendering_res.m_directional_light.m_color.toVector3();
        m_render_scene->m_ambient_light.m_irradiance = global_rendering_res.m_ambient_light.toVector3();
        for (size_t i = 0; i < global_rendering_res.m_point_lights.size(); i++)
        {
            m_render_scene->m_point_light_list.m_lights.push_back(
                {global_rendering_res.m_point_lights[i].m_position, global_rendering_res.m_point_lights[i].m_flux});
        }

        // pipline
        RenderPipelineInitInfo pipeline_init_info;
        pipeline_init_info.render_resource = m_render_resource;
        m_render_pipeline                  = std::make_shared<RenderPipeline>();
        m_render_pipeline->m_vulkan_api    = m_vulkan_api;
        m_render_pipeline->initialize(pipeline_init_info);

        // resource: descriptor layout
        std::static_pointer_cast<RenderResource>(m_render_resource)->m_material_descriptor_set_layout =
            static_cast<RenderPass*>(m_render_pipeline->m_main_camera_pass.get())
                ->m_descriptor_infos[MainCameraPass::LayoutType::_mesh_per_material]
                .layout;
    }

    void RenderSystem::tick(float delta_time)
    {
        // Obtain all the data required for rendering
        processSwapData();

        // Set the "resource" data
        m_render_resource->updatePerFrameBuffer(m_render_scene, m_render_camera);

        // update visible objects
        m_render_scene->updateVisibleObjects(std::static_pointer_cast<RenderResource>(m_render_resource),
                                             m_render_camera);

        // prepare processing data
        m_render_pipeline->preparePassData(m_render_resource);

        g_runtime_global_context.m_debugdraw_manager->tick(delta_time);

        // prepare render command context, Update m_current_command_buffer
        m_vulkan_api->prepareContext();

        // Start rendering
        m_render_pipeline->forwardRender(m_vulkan_api, m_render_resource);
    }

    void RenderSystem::clear()
    {
        if (m_vulkan_api)
        {
            m_vulkan_api->clear();
        }
        m_vulkan_api.reset();

        if (m_render_resource)
        {
            m_render_resource->clear();
        }
        m_render_resource.reset();

        if (m_render_pipeline)
        {
            m_render_pipeline->clear();
        }
        m_render_pipeline.reset();
    }

    std::shared_ptr<VulkanAPI> RenderSystem::getVulkanAPI() const { return m_vulkan_api; }

    RenderSwapContext& RenderSystem::getSwapContext() { return m_swap_context; }

    std::shared_ptr<RenderCamera> RenderSystem::getRenderCamera() const { return m_render_camera; }

    std::shared_ptr<RenderResourceBase> RenderSystem::getRenderResource() const { return m_render_resource; }

    uint32_t RenderSystem::getGuidOfPickedMesh(const Vector2& picked_uv)
    {
        return m_render_pipeline->getGuidOfPickedMesh(picked_uv);
    }

    GObjectID RenderSystem::getGObjectIDByMeshID(uint32_t mesh_id) const
    {
        return m_render_scene->getGObjectIDByMeshID(mesh_id);
    }

    GuidAllocator<GameObjectPartId>& RenderSystem::getGOInstanceIdAllocator()
    {
        return m_render_scene->getInstanceIdAllocator();
    }

    GuidAllocator<MeshSourceDesc>& RenderSystem::getMeshAssetIdAllocator()
    {
        return m_render_scene->getMeshAssetIdAllocator();
    }

    void RenderSystem::setVisibleAxis(std::optional<RenderEntity> axis)
    {
        m_render_scene->m_render_axis = axis;

        if (axis.has_value())
        {
            std::static_pointer_cast<RenderPipeline>(m_render_pipeline)->setAxisVisibleState(true);
        }
        else
        {
            std::static_pointer_cast<RenderPipeline>(m_render_pipeline)->setAxisVisibleState(false);
        }
    }

    void RenderSystem::setSelectedAxis(size_t selected_axis)
    {
        std::static_pointer_cast<RenderPipeline>(m_render_pipeline)->setSelectedAxis(selected_axis);
    }

    void RenderSystem::swapLogicRenderData() { m_swap_context.swapLogicRenderData(); }

    void RenderSystem::initializeUIRenderBackend(WindowUI* window_ui)
    {
        m_render_pipeline->initializeUIRenderBackend(window_ui);
    }

    void RenderSystem::updateEngineContentViewport(float offset_x, float offset_y, float width, float height)
    {
        VkViewport viewport;
        viewport.x        = offset_x;
        viewport.y        = offset_y;
        viewport.width    = width;
        viewport.height   = height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        m_vulkan_api->setViewPort(viewport);
        m_render_camera->setAspect(width / height);
    }

    void RenderSystem::clearForLevelReloading()
    {
        // clear scene
        m_render_scene->clearForLevelReloading();

        // reset camera
        std::shared_ptr<ConfigManager> config_manager = g_runtime_global_context.m_config_manager;
        std::shared_ptr<AssetManager>  asset_manager  = g_runtime_global_context.m_asset_manager;
        GlobalRenderingRes             global_rendering_res;
        const std::string&             global_rendering_res_url = config_manager->getGlobalRenderingResUrl();
        asset_manager->loadAsset(global_rendering_res_url, global_rendering_res);
        m_render_camera->resetData(global_rendering_res.m_camera_config);

        // reset axis
        std::static_pointer_cast<RenderPipeline>(m_render_pipeline)->setAxisVisibleState(false);
    }

    void RenderSystem::createAxis(std::array<RenderEntity, 3> axis_entities, std::array<RenderMeshData, 3> mesh_datas)
    {
        for (int i = 0; i < axis_entities.size(); i++)
        {
            // create buffer and descriptor
            m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, axis_entities[i], mesh_datas[i]);
        }
    }

    void RenderSystem::processSwapData()
    {
        RenderSwapData& swap_data = m_swap_context.getRenderSwapData();

        // update game object if needed

        if (swap_data.m_game_object_resource_desc.has_value())
        {
            while (!swap_data.m_game_object_resource_desc->isEmpty()) ///< Iterate over each object
            {
                GameObjectDesc gobject =
                    swap_data.m_game_object_resource_desc->getNextProcessObject(); ///< get current object

                for (size_t part_index = 0; part_index < gobject.getObjectParts().size();
                     part_index++) ///< Iterate over each all submesh
                {
                    const auto& game_object_part = gobject.getObjectParts()[part_index]; ///< get current submesh

                    // Create an entity for each submesh
                    RenderEntity render_entity;

                    GameObjectPartId part_id = {gobject.getId(), part_index}; ///< instance id: object id, submesh index
                    bool             is_entity_in_scene =
                        m_render_scene->getInstanceIdAllocator().hasElement(part_id); ///< Has it loaded?
                    render_entity.m_instance_id = static_cast<uint32_t>(
                        m_render_scene->getInstanceIdAllocator().allocGuid(part_id)); ///< allocator submesh guid
                    m_render_scene->addInstanceIdToMap(render_entity.m_instance_id,
                                                       gobject.getId()); ///< add id to map

                    render_entity.m_model_matrix =
                        game_object_part.m_transform_desc.m_transform_matrix; ///< set entity model matrix

                    // load vertex data
                    MeshSourceDesc mesh_source = {game_object_part.m_mesh_desc.m_mesh_file};

                    bool is_mesh_loaded =
                        m_render_scene->getMeshAssetIdAllocator().hasElement(mesh_source); ///< Has it loaded?
                    RenderMeshData mesh_data;
                    if (!is_mesh_loaded) ///< Not loaded
                    {
                        mesh_data = m_render_resource->loadMeshData(mesh_source,
                                                                    render_entity.m_bounding_box); ///< load data
                    }
                    else
                    {
                        render_entity.m_bounding_box = m_render_resource->getCachedBoudingBox(mesh_source);
                    }

                    render_entity.m_mesh_asset_id =
                        m_render_scene->getMeshAssetIdAllocator().allocGuid(mesh_source); ///< allocator mesh id

                    if (!is_mesh_loaded)
                    {
                        m_render_resource->uploadGameObjectRenderResource(
                            m_vulkan_api, render_entity, mesh_data); ///< create buffer and descriptor
                    }

                    // load material
                    MaterialSourceDesc material_source;
                    if (game_object_part.m_material_desc.m_with_texture)
                    {
                        material_source = {game_object_part.m_material_desc.m_base_color_texture_file,
                                           game_object_part.m_material_desc.m_normal_texture_file};
                    }

                    bool is_material_loaded =
                        m_render_scene->getMaterialAssetdAllocator().hasElement(material_source); ///< Has it loaded?
                    RenderMaterialData material_data;
                    if (!is_material_loaded) ///< Not loaded
                    {
                        material_data = m_render_resource->loadMaterialData(material_source); ///< load data
                    }

                    render_entity.m_material_asset_id = m_render_scene->getMaterialAssetdAllocator().allocGuid(
                        material_source); ///< allocator material id

                    if (!is_material_loaded)
                    {
                        m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, render_entity, material_data);
                    }

                    // entity
                    if (!is_entity_in_scene) ///< Not loadedS
                    {
                        m_render_scene->m_render_entities.push_back(render_entity); ///< push
                    }
                    else ///< else update
                    {
                        for (auto& entity : m_render_scene->m_render_entities)
                        {
                            if (entity.m_instance_id == render_entity.m_instance_id)
                            {
                                entity = render_entity;
                                break;
                            }
                        }
                    }

                    swap_data.m_game_object_resource_desc->pop(); ///< pop
                }
            }
        }

        // process camera swap data
        if (swap_data.m_camera_swap_data.has_value())
        {
            if (swap_data.m_camera_swap_data->m_fov_x.has_value())
            {
                m_render_camera->setFOVx(*swap_data.m_camera_swap_data->m_fov_x);
            }

            m_swap_context.resetCameraSwapData();
        }
    }
} // namespace VKernel
#include "runtime/function/render/render_system.h"

#include "runtime/function/global/global_context.h"
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

        // resource
        m_render_resource = std::make_shared<RenderResource>();
        m_render_resource->uploadGlobalRenderResource(m_vulkan_api);

        // global rendering resource
        GlobalRenderingRes global_rendering_res;
        const std::string& global_rendering_res_url = config_manager->getGlobalRenderingResUrl();
        asset_manager->loadAsset(global_rendering_res_url, global_rendering_res);

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
        m_render_scene->m_directional_light.m_direction = Vector3(0.0, -1.0, -1.0); // TODO: Not serialized
        m_render_scene->m_directional_light.m_color     = Vector3(1.0, 1.0, 1.0);
        m_render_scene->m_ambient_light.m_irradiance    = Vector3(0.2, 0.2, 0.2);

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

    void RenderSystem::processSwapData()
    {
        RenderSwapData& swap_data = m_swap_context.getRenderSwapData();

        // update global resources if needed
        // m_render_resource->uploadGlobalRenderResource(m_vulkan_api);

        // update game object if needed
        static bool is_mesh_loaded = false;
        if (is_mesh_loaded == false)
        {
            // create entity
            RenderEntity render_entity;
            render_entity.m_mesh_id           = 0; ///< id
            render_entity.m_material_asset_id = 0; ///< id
            render_entity.m_model_matrix =
                Matrix4x4(Vector3(-2.0, 0.0, 5.0), Vector3(0.5, 0.5, 0.5), Quaternion(Vector3(90.0, 90.0, 0.0)));

            RenderEntity render_entity1;
            render_entity1.m_mesh_id           = 1;
            render_entity1.m_material_asset_id = 1;
            render_entity1.m_model_matrix =
                Matrix4x4(Vector3(2.0, 0.0, 6.0), Vector3(0.02, 0.02, 0.02), Quaternion(Vector3(180.0, 90.0, 0.0)));

            RenderEntity render_entity2;
            render_entity2.m_mesh_id           = 0;
            render_entity2.m_material_asset_id = 0;
            render_entity2.m_model_matrix =
                Matrix4x4(Vector3(-3.0, 0.0, 5.0), Vector3(0.5, 0.5, 0.5), Quaternion(Vector3(90.0, 90.0, 0.0)));

            RenderEntity render_entity3;
            render_entity3.m_mesh_id           = 2;
            render_entity3.m_material_asset_id = 2;
            render_entity3.m_model_matrix =
                Matrix4x4(Vector3(0.0, 0.0, 5.0), Vector3(0.2, 0.2, 0.2), Quaternion(Vector3(180.0, 0.0, 0.0)));

            // load vertex and indice data
            MeshSourceDesc mesh_source  = {"engine/asset/objects/basic/viking_room.obj"};
            RenderMeshData mesh_data    = m_render_resource->loadMeshData(mesh_source);
            MeshSourceDesc mesh_source1 = {"engine/asset/objects/basic/Beretta_M92A1.obj"};
            RenderMeshData mesh_data1   = m_render_resource->loadMeshData(mesh_source1);
            MeshSourceDesc mesh_source2 = {"engine/asset/objects/basic/G2A4_Rifle.obj"};
            RenderMeshData mesh_data2   = m_render_resource->loadMeshData(mesh_source2);

            // load buffer and descriptor into the m_vulkan_meshes
            m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, render_entity, mesh_data);
            m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, render_entity1, mesh_data1);
            m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, render_entity2, mesh_data);
            m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, render_entity3, mesh_data2);

            // load material
            MaterialSourceDesc material_source  = {"engine/asset/objects/_textures/viking_room.png"};
            MaterialSourceDesc material_source1 = {"engine/asset/objects/_textures/Beretta_M92A1_c.jpg"};
            MaterialSourceDesc material_source2 = {"engine/asset/objects/_textures/G2A4_Rifle_c.jpg"};

            RenderMaterialData material_data  = m_render_resource->loadMaterialData(material_source);
            RenderMaterialData material_data1 = m_render_resource->loadMaterialData(material_source1);
            RenderMaterialData material_data2 = m_render_resource->loadMaterialData(material_source2);

            m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, render_entity, material_data);
            m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, render_entity1, material_data1);
            m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, render_entity2, material_data);
            m_render_resource->uploadGameObjectRenderResource(m_vulkan_api, render_entity3, material_data2);

            // push entity into the render scene
            m_render_scene->m_render_entities.push_back(render_entity);
            m_render_scene->m_render_entities.push_back(render_entity1);
            m_render_scene->m_render_entities.push_back(render_entity2);
            m_render_scene->m_render_entities.push_back(render_entity3);

            // close
            is_mesh_loaded = true;
        }

        // process camera swap data
        if (swap_data.m_camera_swap_data.has_value())
        {
            if (swap_data.m_camera_swap_data->m_fov_x.has_value())
            {
                m_render_camera->setFOVx(*swap_data.m_camera_swap_data->m_fov_x);
            }

            // m_render_camera->setAspect(m_vulkan_api->getSwapchainInfo().viewport.width /
            //                            m_vulkan_api->getSwapchainInfo().viewport.height);

            m_swap_context.resetCameraSwapData();
        }
    }
} // namespace VKernel
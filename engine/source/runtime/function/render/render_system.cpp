#include "runtime/function/render/render_system.h"

#include "runtime/function/render/vulkan_interface/vulkan_api.h"
#include "runtime/function/render/render_pipeline.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_resource.h"

#include "runtime/resource/res_type/global/global_rendering.h"

namespace VKernel
{

    RenderSystem::~RenderSystem()
    {
        clear();
    }

    void RenderSystem::initialize(std::shared_ptr<WindowSystem> window_system)
    {
        // init
        // vulkan api
        m_vulkan_api = std::make_shared<VulkanAPI>();
        m_vulkan_api->initialize(window_system);

        // pipline
        m_render_pipeline = std::make_shared<RenderPipeline>();
        m_render_pipeline->m_vulkan_api = m_vulkan_api;
        m_render_pipeline->initialize();

        // resource
        m_render_resource = std::make_shared<RenderResource>();

        // camera
        // temp
        GlobalRenderingRes global_rendering_res =
            {
                {
                    ///< camera_config
                    {
                        ///< pose
                        {0.0f, 0.0f, -1.0f}, ///< position
                        {0.0f, 0.0f, 0.0f},  ///< target
                        {0.0f, 0.0f, 1.0f}   ///< up
                    },
                    {640.0f, 400.0f}, ///< aspect
                    1000.0f,          ///< far
                    0.1f              ///< near
                }};

        const CameraPose &camera_pose = global_rendering_res.m_camera_config.m_pose;
        m_render_camera = std::make_shared<RenderCamera>();
        m_render_camera->lookAt(camera_pose.m_position, camera_pose.m_target, camera_pose.m_up);
        m_render_camera->setZFar(global_rendering_res.m_camera_config.m_z_far);
        m_render_camera->setZNear(global_rendering_res.m_camera_config.m_z_near);
        m_render_camera->setAspect(global_rendering_res.m_camera_config.m_aspect.x /
                                   global_rendering_res.m_camera_config.m_aspect.y);
    }

    void RenderSystem::tick(float delta_time)
    {
        // Obtain all the data required for rendering
        processSwapData();

        // Set the "resource" data
        m_render_resource->updatePerFrameBuffer(m_render_camera);

        // prepare processing data
        m_render_pipeline->preparePassData(m_render_resource);

        // prepare render command context, Update m_current_command_buffer
        m_vulkan_api->prepareContext();

        // Start rendering
        m_render_pipeline->forwardRender(m_vulkan_api);
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

    RenderSwapContext &RenderSystem::getSwapContext() { return m_swap_context; }

    std::shared_ptr<RenderCamera> RenderSystem::getRenderCamera() const { return m_render_camera; }

    void RenderSystem::swapLogicRenderData() { m_swap_context.swapLogicRenderData(); }

    void RenderSystem::processSwapData()
    {
        RenderSwapData &swap_data = m_swap_context.getRenderSwapData();

        // process camera swap data
        if (swap_data.m_camera_swap_data.has_value())
        {
            if (swap_data.m_camera_swap_data->m_fov_x.has_value())
            {
                m_render_camera->setFOVx(*swap_data.m_camera_swap_data->m_fov_x);
            }

            m_render_camera->setAspect(m_vulkan_api->getSwapchainInfo().viewport.width / m_vulkan_api->getSwapchainInfo().viewport.height);

            // if (swap_data.m_camera_swap_data->m_view_matrix.has_value())
            // {
            //     m_render_camera->setMainViewMatrix(*swap_data.m_camera_swap_data->m_view_matrix);
            // }

            m_swap_context.resetCameraSwapData();
        }
    }
}
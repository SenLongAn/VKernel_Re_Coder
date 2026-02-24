#pragma once

#include "runtime/function/render/render_pass_base.h"

#include "runtime/core/math/vector2.h"
#include "runtime/function/render/vulkan_interface/vulkan_api.h"

/**
 * The management of each pass, the core of draw, abstract base class
 */
namespace VKernel
{
    class RenderResourceBase;
    class WindowUI;
    class DebugDrawManager;

    struct RenderPipelineInitInfo
    {
        std::shared_ptr<RenderResourceBase> render_resource;
    };

    class RenderPipelineBase
    {

        friend class RenderSystem;

    public:
        virtual ~RenderPipelineBase() {}                               ///< Constructor
        virtual void clear() {};                                       ///< clear
        virtual void initialize(RenderPipelineInitInfo init_info) = 0; ///< init

        virtual void preparePassData(std::shared_ptr<RenderResourceBase> render_resource); ///< prepare processing data

        virtual void forwardRender(std::shared_ptr<VulkanAPI>          vulkan_api,
                                   std::shared_ptr<RenderResourceBase> render_resource)  = 0; ///< forward render core
        virtual void deferredRender(std::shared_ptr<VulkanAPI>          vulkan_api,
                                    std::shared_ptr<RenderResourceBase> render_resource) = 0; ///< deferred

        void initializeUIRenderBackend(); ///< init editor ui Backend: vulkan and glfw

        // get
        virtual std::pair<uint32_t, Vector4> getGuidOfPickedMesh(const Vector2& picked_uv) = 0; ///< get uv submesh guid
        std::shared_ptr<DebugDrawManager>    getDebugManager() { return m_debugdraw_manager; }

    protected:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface

        // pass
        std::shared_ptr<RenderPassBase>   m_directional_light_pass;
        std::shared_ptr<RenderPassBase>   m_point_light_shadow_pass;
        std::shared_ptr<RenderPassBase>   m_main_camera_pass;
        std::shared_ptr<RenderPassBase>   m_color_grading_pass;
        std::shared_ptr<RenderPassBase>   m_fxaa_pass;
        std::shared_ptr<RenderPassBase>   m_tone_mapping_pass;
        std::shared_ptr<RenderPassBase>   m_ui_pass;
        std::shared_ptr<RenderPassBase>   m_combine_ui_pass;
        std::shared_ptr<RenderPassBase>   m_pick_pass;
        std::shared_ptr<DebugDrawManager> m_debugdraw_manager;
    };
} // namespace VKernel
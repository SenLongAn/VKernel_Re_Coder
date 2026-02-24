#pragma once

#include "runtime/function/render/render_entity.h"
#include "runtime/function/render/render_guid_allocator.h"
#include "runtime/function/render/render_pipeline_base.h"
#include "runtime/function/render/render_swap_context.h"

#include <array>
#include <memory>
#include <optional>

/**
 * render core
 */
namespace VKernel
{
    class VulkanAPI;
    class WindowSystem;
    class RenderCamera;
    class RenderScene;
    class RenderResourceBase;

    class RenderSystem ///< render core
    {

    public:
        RenderSystem() = default; ///< Constructor
        ~RenderSystem();          ///< Destructor

        void initialize(std::shared_ptr<WindowSystem> window_system); ///< init
        void tick(float delta_time);                                  ///< tick
        void clear();                                                 ///< clear

        // get
        std::shared_ptr<VulkanAPI>          getVulkanAPI() const;
        RenderSwapContext&                  getSwapContext();
        std::shared_ptr<RenderCamera>       getRenderCamera() const;
        std::shared_ptr<RenderResourceBase> getRenderResource() const;
        std::pair<uint32_t, Vector4>        getGuidOfPickedMesh(const Vector2& picked_uv);
        GObjectID                           getGObjectIDByMeshID(uint32_t mesh_id) const;
        GuidAllocator<GameObjectPartId>&    getGOInstanceIdAllocator();
        GuidAllocator<MeshSourceDesc>&      getMeshAssetIdAllocator();
        std::shared_ptr<RenderPipelineBase> getRenderPipline() const;
        uint32_t                            getGOId() const;
        Vector4                             getPos() const;

        // set
        void setVisibleAxis(std::optional<RenderEntity> axis);
        void setSelectedAxis(size_t selected_axis);
        void setGOId(uint32_t selected_go_id) { m_selected_go_id = selected_go_id; }
        void setPos(Vector4 selected_pos) { m_selected_pos = selected_pos; }

        //
        void swapLogicRenderData(); ///< swap Logic Render Data

        void initializeUIRenderBackend(); ///< init editor ui Backend: vulkan and glfw

        void updateEngineContentViewport(float offset_x,
                                         float offset_y,
                                         float width,
                                         float height); ///< update render viewport

        void clearForLevelReloading(); ///< clear level

        void createAxis(std::array<RenderEntity, 3> axis_entities, std::array<RenderMeshData, 3> mesh_datas);

    private:
        RENDER_PIPELINE_TYPE m_render_pipeline_type {RENDER_PIPELINE_TYPE::FORWARD_PIPELINE}; ///< render mode

        uint32_t m_selected_go_id = 0;
        Vector4  m_selected_pos   = Vector4::ZERO;

        // direct management
        std::shared_ptr<VulkanAPI>          m_vulkan_api;
        std::shared_ptr<RenderCamera>       m_render_camera;
        std::shared_ptr<RenderResourceBase> m_render_resource;
        std::shared_ptr<RenderPipelineBase> m_render_pipeline;
        RenderSwapContext                   m_swap_context;
        std::shared_ptr<RenderScene>        m_render_scene;

    private:
        void processSwapData(); ///< Read data from the renderData of swapContext and pass it to each sub-module of the
                                ///< render process.
    };
} // namespace VKernel
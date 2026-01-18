#pragma once

#include "runtime/function/render/render_common.h"
#include "runtime/function/render/render_pass_base.h"
#include "runtime/function/render/render_resource.h"
#include "runtime/function/render/vulkan_interface/vulkan_api.h"

/**
 * renderpass related, devired class
 */
namespace VKernel
{
    enum ///< attachment count
    {
        _main_camera_pass_gbuffer_a               = 0, ///< GBuffera
        _main_camera_pass_gbuffer_b               = 1, ///< GBufferb
        _main_camera_pass_gbuffer_c               = 2, ///< GBufferc
        _main_camera_pass_backup_buffer_odd       = 3, ///< scene ui
        _main_camera_pass_backup_buffer_even      = 4, ///< eidtor ui
        _main_camera_pass_depth                   = 5, ///< depth
        _main_camera_pass_swap_chain_image        = 6, ///< color
        _main_camera_pass_attachment_count        = 7, ///< sum attachment count
        _main_camera_pass_custom_attachment_count = 5  ///< custom attachment count
    };

    enum ///< subpass
    {
        _main_camera_subpass_basepass = 0,      ///< deferred GBuffer
        _main_camera_subpass_deferred_lighting, ///< deferred light
        _main_camera_subpass_forward_lighting,  ///< forward render
        _main_camera_subpass_fxaa,              ///< fxaa
        _main_camera_subpass_ui,                ///< ui
        _main_camera_subpass_combine_ui,        ///< combine sence and editorUI
        _main_camera_subpass_count              ///< sum subpass count
    };

    struct VisiableNodes
    {
        std::vector<RenderMeshNode>* p_directional_light_visible_mesh_nodes {nullptr}; ///< directional light
        std::vector<RenderMeshNode>* p_point_lights_visible_mesh_nodes {nullptr};      ///< point light
        std::vector<RenderMeshNode>* p_main_camera_visible_mesh_nodes {nullptr};       ///< main camera
        RenderAxisNode*              p_axis_node {nullptr};                            ///< axis
    };

    class RenderPass : public RenderPassBase ///< attachment-related
    {
    public:
        struct FrameBufferAttachment
        {
            VkImage        image = VK_NULL_HANDLE;
            VkDeviceMemory mem   = VK_NULL_HANDLE;
            VkImageView    view  = VK_NULL_HANDLE;
            VkFormat       format;
            VmaAllocation  allocation;
        };

        struct Framebuffer ///< renderpass, framebuffer, attachment
        {
            int                                width;
            int                                height;
            VkRenderPass                       render_pass = VK_NULL_HANDLE;
            VkFramebuffer                      framebuffer = VK_NULL_HANDLE;
            std::vector<FrameBufferAttachment> attachments;
        };

        struct RenderPipelineBase ///< pipelineLayout, pipline
        {
            VkPipelineLayout layout   = VK_NULL_HANDLE;
            VkPipeline       pipeline = VK_NULL_HANDLE;
        };

        struct Descriptor
        {
            VkDescriptorSetLayout layout         = VK_NULL_HANDLE;
            VkDescriptorSet       descriptor_set = VK_NULL_HANDLE;
        };

    public:
        std::vector<RenderPipelineBase> m_render_pipelines;
        Framebuffer                     m_framebuffer;

        static VisiableNodes  m_visiable_nodes;
        GlobalRenderResource* m_global_render_resource {nullptr};

        std::vector<Descriptor> m_descriptor_infos;

    public:
        void initialize(const RenderPassInitInfo* init_info) override;
        void postInitialize() override;

        virtual VkRenderPass getRenderPass() const;

        virtual void draw();

        virtual std::vector<VkImageView> getFramebufferImageViews() const;
    };
} // namespace VKernel
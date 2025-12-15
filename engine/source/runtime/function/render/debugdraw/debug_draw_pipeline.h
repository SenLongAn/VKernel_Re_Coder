#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"

/**
 * debug draw renderpass related
 */
namespace VKernel
{
    struct DebugDrawFrameBufferAttachment ///< attachment-related
    {
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory mem = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
        VkFormat format;
    };

    struct DebugDrawFramebuffer ///< renderpass, framebuffer, attachment
    {
        int width;
        int height;
        VkRenderPass render_pass = VK_NULL_HANDLE; ///< render pass

        ///< A render pass can have multiple framebuffer and attachment
        std::vector<VkFramebuffer> framebuffers;
        std::vector<DebugDrawFrameBufferAttachment> attachments;
    };

    struct DebugDrawPipelineBase ///< pipelineLayout, pipline
    {
        VkPipelineLayout layout = VK_NULL_HANDLE;
        VkPipeline pipeline = VK_NULL_HANDLE;
    };

    enum DebugDrawPipelineType : uint8_t ///< pipline type (AssemblyState and DepthStencilState)
    {
        _debug_draw_pipeline_type_point = 0,
        _debug_draw_pipeline_type_line,
        _debug_draw_pipeline_type_triangle,
        _debug_draw_pipeline_type_point_no_depth_test,
        _debug_draw_pipeline_type_line_no_depth_test,
        _debug_draw_pipeline_type_triangle_no_depth_test,
        _debug_draw_pipeline_type_count,
    };

    class DebugDrawPipeline ///< Debug Draw Pipeline
    {

    public:
        DebugDrawPipelineType m_pipeline_type; ///<  pipline type

    public:
        DebugDrawPipeline(DebugDrawPipelineType pipelineType) { m_pipeline_type = pipelineType; } ///< Constructor
        void initialize(const VkAttachmentLoadOp& load_op, const VkImageLayout& initial_layout, const VkImageLayout& initial_layout_depth);                                                                        ///< init
        void destory() {}                                                                         /// clear

        // get
        const DebugDrawPipelineBase &getPipeline() const;
        const DebugDrawFramebuffer &getFramebuffer() const;

        void recreateAfterSwapchain(); ///< destory and recreate framebuffer

    private:
        // What needs to be accomplished in the pipeline class
        void setupFramebuffer();
        void setupRenderPass(const VkAttachmentLoadOp& load_op, const VkImageLayout& initial_layout, const VkImageLayout& initial_layout_depth);
        void setupDescriptorLayout();
        void setupPipelines();

    private:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface

        // What needs to be accomplished in the pipeline class
        VkDescriptorSetLayout m_descriptor_layout;
        DebugDrawFramebuffer m_framebuffer;
        std::vector<DebugDrawPipelineBase> m_render_pipelines; ///< A render pass can have multiple pipelines
    };
}
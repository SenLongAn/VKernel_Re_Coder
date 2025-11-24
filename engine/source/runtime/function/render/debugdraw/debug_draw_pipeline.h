#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"

/**
 * Equivalent to the renderpass in Vulkan
 */
namespace VKernel
{
    struct DebugDrawFrameBufferAttachment ///< attachment-related
    {
        VkImage image = nullptr;
        VkDeviceMemory mem = nullptr;
        VkImageView view = nullptr;
        VkFormat format;
    };

    struct DebugDrawFramebuffer ///< renderpass, framebuffer, attachment
    {
        int           width;
        int           height;
        VkRenderPass render_pass = nullptr; ///< render pass

        ///< A render pass can have multiple framebuffer and attachment
        std::vector<VkFramebuffer> framebuffers;
        std::vector<DebugDrawFrameBufferAttachment> attachments;
    };

    struct DebugDrawPipelineBase ///< pipelineLayout, pipline
    {
        VkPipelineLayout layout = nullptr;
        VkPipeline pipeline = nullptr;
    };
    
    enum DebugDrawPipelineType : uint8_t ///< pipline type (AssemblyState and DepthStencilState)
    {
        _debug_draw_pipeline_type_point_no_depth_test = 0,
        _debug_draw_pipeline_type_line_no_depth_test,
        _debug_draw_pipeline_type_triangle_no_depth_test,
        _debug_draw_pipeline_type_count,
    };

    class DebugDrawPipeline ///< Debug Draw Pipeline
    {

    public:
        DebugDrawPipelineType m_pipeline_type; ///<  pipline type

    public:
        DebugDrawPipeline(DebugDrawPipelineType pipelineType) {m_pipeline_type = pipelineType; }  ///< Constructor
        void initialize(); ///< init

        // get
        const DebugDrawPipelineBase& getPipeline() const;
        const DebugDrawFramebuffer& getFramebuffer() const;

    private:
        // What needs to be accomplished in the pipeline class
        void setupFramebuffer();
        void setupRenderPass();
        void setupPipelines();
    
    private:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface

        // What needs to be accomplished in the pipeline class
        DebugDrawFramebuffer m_framebuffer;
        std::vector<DebugDrawPipelineBase> m_render_pipelines; ///< A render pass can have multiple pipelines
    };
}
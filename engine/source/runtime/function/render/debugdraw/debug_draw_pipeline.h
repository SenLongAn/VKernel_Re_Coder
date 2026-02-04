#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"

/**
 * debug draw renderpass related
 */
namespace VKernel
{

    struct DebugDrawPipelineBase ///< pipelineLayout, pipline
    {
        VkPipelineLayout layout   = VK_NULL_HANDLE;
        VkPipeline       pipeline = VK_NULL_HANDLE;
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
        void initialize(const VkAttachmentLoadOp& load_op,
                        const VkImageLayout&      initial_layout,
                        const VkImageLayout&      initial_layout_depth,
                        const VkRenderPass&       renderpass); ///< init
        void destory() {}                                /// clear

        // get
        const DebugDrawPipelineBase& getPipeline() const;

    private:
        // What needs to be accomplished in the pipeline class
        void setupFramebuffer();
        void setupDescriptorLayout();
        void setupPipelines();

    private:
        std::shared_ptr<VulkanAPI> m_vulkan_api; ///< Vulkan interface

        // What needs to be accomplished in the pipeline class
        VkDescriptorSetLayout              m_descriptor_layout;
        VkRenderPass                       render_pass = VK_NULL_HANDLE;
        std::vector<DebugDrawPipelineBase> m_render_pipelines; ///< A render pass can have multiple pipelines
    };
} // namespace VKernel
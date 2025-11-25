#include "runtime/function/render/debugdraw/debug_draw_pipeline.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/debugdraw/debug_draw_primitive.h"

#include <debugdraw_vert.h>
#include <debugdraw_frag.h>

#include <array>
#include <iostream>

namespace VKernel
{
    void DebugDrawPipeline::initialize()
    {
        m_vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();

        setupRenderPass();
        setupFramebuffer();
        setupPipelines();
    }
    
    const DebugDrawPipelineBase &DebugDrawPipeline::getPipeline() const
    {
        return m_render_pipelines[0];
    }
    const DebugDrawFramebuffer &DebugDrawPipeline::getFramebuffer() const
    {
        return m_framebuffer;
    }

    void DebugDrawPipeline::setupRenderPass()
    {
        // color attachment
        VkAttachmentDescription color_attachment_description{};
        color_attachment_description.format = m_vulkan_api->getSwapchainInfo().image_format;
        color_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_reference{};
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        // Subpass
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_reference;

        // Subpass Dependency
        VkSubpassDependency dependencies[1] = {};
        VkSubpassDependency& debug_draw_dependency = dependencies[0];
        debug_draw_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        debug_draw_dependency.dstSubpass = 0;
        debug_draw_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        debug_draw_dependency.srcAccessMask = 0;
        debug_draw_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        debug_draw_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        
        // RenderPass
        VkRenderPassCreateInfo renderpass_create_info{};
        renderpass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderpass_create_info.attachmentCount = 1;
        renderpass_create_info.pAttachments = &color_attachment_description;
        renderpass_create_info.subpassCount = 1;
        renderpass_create_info.pSubpasses = &subpass;
        renderpass_create_info.dependencyCount = 1;
        renderpass_create_info.pDependencies = dependencies;
        
        if (vkCreateRenderPass(m_vulkan_api->getLogicDevice(), &renderpass_create_info, nullptr, &m_framebuffer.render_pass) != VK_SUCCESS)
        {
            throw std::runtime_error("create inefficient pick render pass");
        }
    }

    void DebugDrawPipeline::setupFramebuffer()
    {
        const std::vector<VkImageView> imageViews = m_vulkan_api->getSwapchainInfo().imageViews;
        
        m_framebuffer.framebuffers.resize(imageViews.size());

        for (size_t i = 0; i < m_framebuffer.framebuffers.size(); i++) {

            VkImageView attachments[] = {imageViews[i]};

            VkFramebufferCreateInfo framebuffer_create_info{};
            framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_create_info.renderPass = m_framebuffer.render_pass;
            framebuffer_create_info.attachmentCount = sizeof(attachments) / sizeof(attachments[0]);
            framebuffer_create_info.pAttachments = attachments;
            framebuffer_create_info.width = m_vulkan_api->getSwapchainInfo().extent.width;
            framebuffer_create_info.height = m_vulkan_api->getSwapchainInfo().extent.height;
            framebuffer_create_info.layers = 1;
            
            if (vkCreateFramebuffer(m_vulkan_api->getLogicDevice(), &framebuffer_create_info, nullptr, &m_framebuffer.framebuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("create inefficient pick framebuffer");
            }
        }
    }

    void DebugDrawPipeline::setupPipelines()
    {
        // Pipeline Layout
        m_render_pipelines.resize(1);

        VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
        pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_create_info.setLayoutCount = 0;
        pipeline_layout_create_info.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(m_vulkan_api->getLogicDevice(), &pipeline_layout_create_info, nullptr, &m_render_pipelines[0].layout) != VK_SUCCESS)
        {
            throw std::runtime_error("create mesh inefficient pick pipeline layout");
        }

        // Shader Module
        VkShaderModule vert_shader_module = m_vulkan_api->createShaderModule(DEBUGDRAW_VERT);
        VkShaderModule frag_shader_module = m_vulkan_api->createShaderModule(DEBUGDRAW_FRAG);

        VkPipelineShaderStageCreateInfo vert_pipeline_shader_stage_create_info{};
        vert_pipeline_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_pipeline_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vert_pipeline_shader_stage_create_info.module = vert_shader_module;
        vert_pipeline_shader_stage_create_info.pName = "main";

        VkPipelineShaderStageCreateInfo frag_pipeline_shader_stage_create_info{};
        frag_pipeline_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_pipeline_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_pipeline_shader_stage_create_info.module = frag_shader_module;
        frag_pipeline_shader_stage_create_info.pName = "main";

        VkPipelineShaderStageCreateInfo shader_stages[] = { vert_pipeline_shader_stage_create_info,
                                                           frag_pipeline_shader_stage_create_info };

        // VertexInput state
        auto vertex_binding_descriptions = DebugDrawVertex::getBindingDescriptions();
        auto vertex_attribute_descriptions = DebugDrawVertex::getAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};
        vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state_create_info.vertexBindingDescriptionCount = 1;
        vertex_input_state_create_info.pVertexBindingDescriptions = &vertex_binding_descriptions;
        vertex_input_state_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_attribute_descriptions.size());
        vertex_input_state_create_info.pVertexAttributeDescriptions = vertex_attribute_descriptions.data();

        // Assembly State
        VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info{};
        input_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

        // Viewport State
        VkPipelineViewportStateCreateInfo viewport_state_create_info{};
        viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_create_info.viewportCount = 1;
        SwapChainDesc swapChainInfo = m_vulkan_api->getSwapchainInfo();
        viewport_state_create_info.pViewports = &swapChainInfo.viewport;
        viewport_state_create_info.scissorCount = 1;
        viewport_state_create_info.pScissors = &swapChainInfo.scissor;

        // Rasterization State
        VkPipelineRasterizationStateCreateInfo rasterization_state_create_info{};
        rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization_state_create_info.depthClampEnable = VK_FALSE;
        rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
        rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
        rasterization_state_create_info.lineWidth = 1.0f;
        rasterization_state_create_info.cullMode = VK_CULL_MODE_NONE;
        rasterization_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterization_state_create_info.depthBiasEnable = VK_FALSE;

        // Multisample State
        VkPipelineMultisampleStateCreateInfo multisample_state_create_info{};
        multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisample_state_create_info.sampleShadingEnable = VK_FALSE;
        multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // ColorBlendAttachment State
        VkPipelineColorBlendAttachmentState color_blend_attachment_state{};
        color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment_state.blendEnable = VK_FALSE;

        // ColorBlend State
        VkPipelineColorBlendStateCreateInfo color_blend_state_create_info{};
        color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_state_create_info.logicOpEnable = VK_FALSE;
        color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
        color_blend_state_create_info.attachmentCount = 1;
        color_blend_state_create_info.pAttachments = &color_blend_attachment_state;
        color_blend_state_create_info.blendConstants[0] = 0.0f;
        color_blend_state_create_info.blendConstants[1] = 0.0f;
        color_blend_state_create_info.blendConstants[2] = 0.0f;
        color_blend_state_create_info.blendConstants[3] = 0.0f;

        // Dynamic State
        VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
        dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.dynamicStateCount = 2;
        dynamic_state_create_info.pDynamicStates = dynamic_states;

        // Pipeline info
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shader_stages;
        pipelineInfo.pVertexInputState = &vertex_input_state_create_info;
        pipelineInfo.pInputAssemblyState = &input_assembly_create_info;
        pipelineInfo.pViewportState = &viewport_state_create_info;
        pipelineInfo.pRasterizationState = &rasterization_state_create_info;
        pipelineInfo.pMultisampleState = &multisample_state_create_info;
        pipelineInfo.pColorBlendState = &color_blend_state_create_info;
        pipelineInfo.pDynamicState = &dynamic_state_create_info;
        pipelineInfo.layout = m_render_pipelines[0].layout;
        pipelineInfo.renderPass = m_framebuffer.render_pass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        // Adjust according to the pipeline type
        if (m_pipeline_type == _debug_draw_pipeline_type_point_no_depth_test)
        {
            input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        }
        else if (m_pipeline_type == _debug_draw_pipeline_type_line_no_depth_test)
        {
            input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        }
        else if (m_pipeline_type == _debug_draw_pipeline_type_triangle_no_depth_test)
        {
            input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        }

        // create Pipeline
        if (vkCreateGraphicsPipelines(m_vulkan_api->getLogicDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_render_pipelines[0].pipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("create debug draw graphics pipeline");
        }

        m_vulkan_api->destroyShaderModule(vert_shader_module);
        m_vulkan_api->destroyShaderModule(frag_shader_module);
    }
}
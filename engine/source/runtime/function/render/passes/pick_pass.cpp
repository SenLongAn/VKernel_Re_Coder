#include "runtime/function/render/passes/pick_pass.h"

#include "runtime/function/render/render_mesh.h"
#include "runtime/function/render/vulkan_interface/vulkan_api.h"
#include "runtime/function/render/vulkan_interface/vulkan_util.h"

#include "runtime/function/render/render_helper.h"

#include <mesh_inefficient_pick_frag.h>
#include <mesh_inefficient_pick_vert.h>

#include <map>
#include <stdexcept>

namespace VKernel
{
    void PickPass::initialize(const RenderPassInitInfo* init_info)
    {
        RenderPass::initialize(nullptr);

        // init
        setupAttachments();
        setupRenderPass();
        setupFramebuffer();
        setupDescriptorSetLayout();
        setupPipelines();
        setupDescriptorSet();
    }
    void PickPass::postInitialize() {}

    void PickPass::preparePassData(std::shared_ptr<RenderResourceBase> render_resource)
    {
        // load data
        const RenderResource* vulkan_resource = static_cast<const RenderResource*>(render_resource.get());
        if (vulkan_resource)
        {
            _mesh_inefficient_pick_perframe_storage_buffer_object.proj_view_matrix =
                vulkan_resource->m_mesh_inefficient_pick_perframe_storage_buffer_object.proj_view_matrix;
        }
    }
    void PickPass::draw() {}

    void PickPass::setupAttachments()
    {
        // create render target
        m_framebuffer.attachments.resize(1);
        m_framebuffer.attachments[0].format = VK_FORMAT_R32_UINT;

        m_vulkan_api->createImage(m_vulkan_api->getSwapchainInfo().extent.width,
                                  m_vulkan_api->getSwapchainInfo().extent.height,
                                  m_framebuffer.attachments[0].format,
                                  VK_IMAGE_TILING_OPTIMAL,
                                  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                  m_framebuffer.attachments[0].image,
                                  m_framebuffer.attachments[0].mem,
                                  0,
                                  1,
                                  1);
        m_vulkan_api->createImageView(m_framebuffer.attachments[0].image,
                                      m_framebuffer.attachments[0].format,
                                      VK_IMAGE_ASPECT_COLOR_BIT,
                                      VK_IMAGE_VIEW_TYPE_2D,
                                      1,
                                      1,
                                      m_framebuffer.attachments[0].view);
    }
    void PickPass::setupRenderPass()
    {
        // Attachment Description
        VkAttachmentDescription color_attachment_description {};
        color_attachment_description.format         = m_framebuffer.attachments[0].format;
        color_attachment_description.samples        = VK_SAMPLE_COUNT_1_BIT;
        color_attachment_description.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment_description.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment_description.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment_description.initialLayout  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        color_attachment_description.finalLayout    = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

        VkAttachmentDescription depth_attachment_description {};
        depth_attachment_description.format         = m_vulkan_api->getDepthImageInfo().depth_image_format;
        depth_attachment_description.samples        = VK_SAMPLE_COUNT_1_BIT;
        depth_attachment_description.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depth_attachment_description.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment_description.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment_description.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        depth_attachment_description.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference color_attachment_reference {};
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depth_attachment_reference {};
        depth_attachment_reference.attachment = 1;
        depth_attachment_reference.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount    = 1;
        subpass.pColorAttachments       = &color_attachment_reference;
        subpass.pDepthStencilAttachment = &depth_attachment_reference;

        VkAttachmentDescription attachments[2] = {color_attachment_description, depth_attachment_description};
        VkRenderPassCreateInfo  renderpass_create_info {};
        renderpass_create_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderpass_create_info.attachmentCount = sizeof(attachments) / sizeof(attachments[0]);
        renderpass_create_info.pAttachments    = attachments;
        renderpass_create_info.subpassCount    = 1;
        renderpass_create_info.pSubpasses      = &subpass;
        renderpass_create_info.dependencyCount = 0;
        renderpass_create_info.pDependencies   = NULL;

        if (vkCreateRenderPass(
                m_vulkan_api->getLogicDevice(), &renderpass_create_info, nullptr, &m_framebuffer.render_pass) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("create inefficient pick render pass");
        }
    }
    void PickPass::setupFramebuffer()
    {
        VkImageView attachments[2] = {m_framebuffer.attachments[0].view,
                                      m_vulkan_api->getDepthImageInfo().depth_image_view};

        VkFramebufferCreateInfo framebuffer_create_info {};
        framebuffer_create_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.renderPass      = m_framebuffer.render_pass;
        framebuffer_create_info.attachmentCount = sizeof(attachments) / sizeof(attachments[0]);
        framebuffer_create_info.pAttachments    = attachments;
        framebuffer_create_info.width           = m_vulkan_api->getSwapchainInfo().extent.width;
        framebuffer_create_info.height          = m_vulkan_api->getSwapchainInfo().extent.height;
        framebuffer_create_info.layers          = 1;

        if (vkCreateFramebuffer(
                m_vulkan_api->getLogicDevice(), &framebuffer_create_info, nullptr, &m_framebuffer.framebuffer) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("create inefficient pick framebuffer");
        }
    }
    void PickPass::setupDescriptorSetLayout()
    {
        m_descriptor_infos.resize(1);

        // DescriptorSetLayoutBinding
        VkDescriptorSetLayoutBinding mesh_inefficient_pick_global_layout_bindings[2];

        VkDescriptorSetLayoutBinding& mesh_inefficient_pick_global_layout_perframe_storage_buffer_binding =
            mesh_inefficient_pick_global_layout_bindings[0];
        mesh_inefficient_pick_global_layout_perframe_storage_buffer_binding.binding = 0;
        mesh_inefficient_pick_global_layout_perframe_storage_buffer_binding.descriptorType =
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        mesh_inefficient_pick_global_layout_perframe_storage_buffer_binding.descriptorCount = 1;
        mesh_inefficient_pick_global_layout_perframe_storage_buffer_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        mesh_inefficient_pick_global_layout_perframe_storage_buffer_binding.pImmutableSamplers = NULL;

        VkDescriptorSetLayoutBinding& mesh_inefficient_pick_global_layout_perdrawcall_storage_buffer_binding =
            mesh_inefficient_pick_global_layout_bindings[1];
        mesh_inefficient_pick_global_layout_perdrawcall_storage_buffer_binding.binding = 1;
        mesh_inefficient_pick_global_layout_perdrawcall_storage_buffer_binding.descriptorType =
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        mesh_inefficient_pick_global_layout_perdrawcall_storage_buffer_binding.descriptorCount = 1;
        mesh_inefficient_pick_global_layout_perdrawcall_storage_buffer_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        mesh_inefficient_pick_global_layout_perdrawcall_storage_buffer_binding.pImmutableSamplers = NULL;

        VkDescriptorSetLayoutCreateInfo mesh_inefficient_pick_global_layout_create_info;
        mesh_inefficient_pick_global_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        mesh_inefficient_pick_global_layout_create_info.pNext = NULL;
        mesh_inefficient_pick_global_layout_create_info.flags = 0;
        mesh_inefficient_pick_global_layout_create_info.bindingCount =
            (sizeof(mesh_inefficient_pick_global_layout_bindings) /
             sizeof(mesh_inefficient_pick_global_layout_bindings[0]));
        mesh_inefficient_pick_global_layout_create_info.pBindings = mesh_inefficient_pick_global_layout_bindings;

        if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_api->getLogicDevice(),
                                                      &mesh_inefficient_pick_global_layout_create_info,
                                                      nullptr,
                                                      &m_descriptor_infos[0].layout))

        {
            throw std::runtime_error("create mesh inefficient pick global layout");
        }
    }
    void PickPass::setupPipelines()
    {
        m_render_pipelines.resize(1);

        // Pipeline Layout
        VkDescriptorSetLayout descriptorset_layouts[] = {m_descriptor_infos[0].layout};

        VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
        pipeline_layout_create_info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_create_info.setLayoutCount = sizeof(descriptorset_layouts) / sizeof(descriptorset_layouts[0]);
        pipeline_layout_create_info.pSetLayouts    = descriptorset_layouts;

        if (vkCreatePipelineLayout(
                m_vulkan_api->getLogicDevice(), &pipeline_layout_create_info, nullptr, &m_render_pipelines[0].layout) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("create mesh inefficient pick pipeline layout");
        }

        // Shader Module
        VkShaderModule vert_shader_module = m_vulkan_api->createShaderModule(MESH_INEFFICIENT_PICK_VERT);
        VkShaderModule frag_shader_module = m_vulkan_api->createShaderModule(MESH_INEFFICIENT_PICK_FRAG);

        VkPipelineShaderStageCreateInfo vert_pipeline_shader_stage_create_info {};
        vert_pipeline_shader_stage_create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_pipeline_shader_stage_create_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
        vert_pipeline_shader_stage_create_info.module = vert_shader_module;
        vert_pipeline_shader_stage_create_info.pName  = "main";

        VkPipelineShaderStageCreateInfo frag_pipeline_shader_stage_create_info {};
        frag_pipeline_shader_stage_create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_pipeline_shader_stage_create_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_pipeline_shader_stage_create_info.module = frag_shader_module;
        frag_pipeline_shader_stage_create_info.pName  = "main";

        VkPipelineShaderStageCreateInfo shader_stages[] = {vert_pipeline_shader_stage_create_info,
                                                           frag_pipeline_shader_stage_create_info};

        // VertexInput state
        auto                                 vertex_binding_descriptions   = MeshVertex::getBindingDescriptions();
        auto                                 vertex_attribute_descriptions = MeshVertex::getAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info {};
        vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state_create_info.vertexBindingDescriptionCount   = 1;
        vertex_input_state_create_info.pVertexBindingDescriptions      = &vertex_binding_descriptions[0];
        vertex_input_state_create_info.vertexAttributeDescriptionCount = 1;
        vertex_input_state_create_info.pVertexAttributeDescriptions    = &vertex_attribute_descriptions[0];

        // Assembly State
        VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info {};
        input_assembly_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

        // Viewport State
        VkPipelineViewportStateCreateInfo viewport_state_create_info {};
        viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_create_info.viewportCount = 1;
        viewport_state_create_info.scissorCount  = 1;

        // Rasterization State
        VkPipelineRasterizationStateCreateInfo rasterization_state_create_info {};
        rasterization_state_create_info.sType            = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization_state_create_info.depthClampEnable = VK_FALSE;
        rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
        rasterization_state_create_info.polygonMode             = VK_POLYGON_MODE_FILL;
        rasterization_state_create_info.lineWidth               = 1.0f;
        rasterization_state_create_info.cullMode                = VK_CULL_MODE_NONE;
        rasterization_state_create_info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
        rasterization_state_create_info.depthBiasEnable         = VK_FALSE;
        rasterization_state_create_info.depthBiasConstantFactor = 0.0f;
        rasterization_state_create_info.depthBiasClamp          = 0.0f;
        rasterization_state_create_info.depthBiasSlopeFactor    = 0.0f;

        // Multisample State
        VkPipelineMultisampleStateCreateInfo multisample_state_create_info {};
        multisample_state_create_info.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisample_state_create_info.sampleShadingEnable  = VK_FALSE;
        multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // ColorBlend State
        VkPipelineColorBlendAttachmentState color_blend_attachment_state {};
        color_blend_attachment_state.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT;
        color_blend_attachment_state.blendEnable         = VK_FALSE;
        color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.colorBlendOp        = VK_BLEND_OP_ADD;
        color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.alphaBlendOp        = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo color_blend_state_create_info {};
        color_blend_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_state_create_info.logicOpEnable     = VK_FALSE;
        color_blend_state_create_info.logicOp           = VK_LOGIC_OP_COPY;
        color_blend_state_create_info.attachmentCount   = 1;
        color_blend_state_create_info.pAttachments      = &color_blend_attachment_state;
        color_blend_state_create_info.blendConstants[0] = 0.0f;
        color_blend_state_create_info.blendConstants[1] = 0.0f;
        color_blend_state_create_info.blendConstants[2] = 0.0f;
        color_blend_state_create_info.blendConstants[3] = 0.0f;

        // depth state
        VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info {};
        depth_stencil_create_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depth_stencil_create_info.depthTestEnable       = VK_TRUE;
        depth_stencil_create_info.depthWriteEnable      = VK_TRUE;
        depth_stencil_create_info.depthCompareOp        = VK_COMPARE_OP_LESS;
        depth_stencil_create_info.depthBoundsTestEnable = VK_FALSE;
        depth_stencil_create_info.stencilTestEnable     = VK_FALSE;

        // Dynamic State
        VkDynamicState                   dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamic_state_create_info {};
        dynamic_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.dynamicStateCount = 2;
        dynamic_state_create_info.pDynamicStates    = dynamic_states;

        // Pipeline info
        VkGraphicsPipelineCreateInfo pipelineInfo {};
        pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount          = 2;
        pipelineInfo.pStages             = shader_stages;
        pipelineInfo.pVertexInputState   = &vertex_input_state_create_info;
        pipelineInfo.pInputAssemblyState = &input_assembly_create_info;
        pipelineInfo.pViewportState      = &viewport_state_create_info;
        pipelineInfo.pRasterizationState = &rasterization_state_create_info;
        pipelineInfo.pMultisampleState   = &multisample_state_create_info;
        pipelineInfo.pColorBlendState    = &color_blend_state_create_info;
        pipelineInfo.pDynamicState       = &dynamic_state_create_info;
        pipelineInfo.pDepthStencilState  = &depth_stencil_create_info;
        pipelineInfo.layout              = m_render_pipelines[0].layout;
        pipelineInfo.renderPass          = m_framebuffer.render_pass;
        pipelineInfo.subpass             = 0;
        pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_vulkan_api->getLogicDevice(),
                                      VK_NULL_HANDLE,
                                      1,
                                      &pipelineInfo,
                                      nullptr,
                                      &m_render_pipelines[0].pipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("create mesh inefficient pick graphics pipeline");
        }

        // destory shader module
        m_vulkan_api->destroyShaderModule(vert_shader_module);
        m_vulkan_api->destroyShaderModule(frag_shader_module);
    }
    void PickPass::setupDescriptorSet()
    {
        // DescriptorSet
        VkDescriptorSetAllocateInfo mesh_inefficient_pick_global_descriptor_set_alloc_info;
        mesh_inefficient_pick_global_descriptor_set_alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        mesh_inefficient_pick_global_descriptor_set_alloc_info.pNext = NULL;
        mesh_inefficient_pick_global_descriptor_set_alloc_info.descriptorPool     = m_vulkan_api->getDescriptorPool();
        mesh_inefficient_pick_global_descriptor_set_alloc_info.descriptorSetCount = 1;
        mesh_inefficient_pick_global_descriptor_set_alloc_info.pSetLayouts        = &m_descriptor_infos[0].layout;

        if (VK_SUCCESS != vkAllocateDescriptorSets(m_vulkan_api->getLogicDevice(),
                                                   &mesh_inefficient_pick_global_descriptor_set_alloc_info,
                                                   &m_descriptor_infos[0].descriptor_set))
        {
            throw std::runtime_error("allocate mesh inefficient pick global descriptor set");
        }

        // DescriptorSet bind buffer
        VkDescriptorBufferInfo mesh_inefficient_pick_perframe_storage_buffer_info = {};
        mesh_inefficient_pick_perframe_storage_buffer_info.offset                 = 0;
        mesh_inefficient_pick_perframe_storage_buffer_info.range =
            sizeof(MeshInefficientPickPerframeStorageBufferObject);
        mesh_inefficient_pick_perframe_storage_buffer_info.buffer =
            m_global_render_resource->_storage_buffer._global_upload_ringbuffer;

        VkDescriptorBufferInfo mesh_inefficient_pick_perdrawcall_storage_buffer_info = {};
        mesh_inefficient_pick_perdrawcall_storage_buffer_info.offset                 = 0;
        mesh_inefficient_pick_perdrawcall_storage_buffer_info.range =
            sizeof(MeshInefficientPickPerdrawcallStorageBufferObject);
        mesh_inefficient_pick_perdrawcall_storage_buffer_info.buffer =
            m_global_render_resource->_storage_buffer._global_upload_ringbuffer;
        assert(mesh_inefficient_pick_perdrawcall_storage_buffer_info.range <
               m_global_render_resource->_storage_buffer._max_storage_buffer_range);

        VkWriteDescriptorSet mesh_descriptor_writes_info[2];

        mesh_descriptor_writes_info[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        mesh_descriptor_writes_info[0].pNext           = NULL;
        mesh_descriptor_writes_info[0].dstSet          = m_descriptor_infos[0].descriptor_set;
        mesh_descriptor_writes_info[0].dstBinding      = 0;
        mesh_descriptor_writes_info[0].dstArrayElement = 0;
        mesh_descriptor_writes_info[0].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        mesh_descriptor_writes_info[0].descriptorCount = 1;
        mesh_descriptor_writes_info[0].pBufferInfo     = &mesh_inefficient_pick_perframe_storage_buffer_info;

        mesh_descriptor_writes_info[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        mesh_descriptor_writes_info[1].pNext           = NULL;
        mesh_descriptor_writes_info[1].dstSet          = m_descriptor_infos[0].descriptor_set;
        mesh_descriptor_writes_info[1].dstBinding      = 1;
        mesh_descriptor_writes_info[1].dstArrayElement = 0;
        mesh_descriptor_writes_info[1].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        mesh_descriptor_writes_info[1].descriptorCount = 1;
        mesh_descriptor_writes_info[1].pBufferInfo     = &mesh_inefficient_pick_perdrawcall_storage_buffer_info;

        vkUpdateDescriptorSets(m_vulkan_api->getLogicDevice(),
                               sizeof(mesh_descriptor_writes_info) / sizeof(mesh_descriptor_writes_info[0]),
                               mesh_descriptor_writes_info,
                               0,
                               NULL);
    }
    void PickPass::recreateFramebuffer()
    {
        // destory
        for (size_t i = 0; i < m_framebuffer.attachments.size(); i++)
        {
            vkDestroyImage(m_vulkan_api->getLogicDevice(), m_framebuffer.attachments[i].image, nullptr);
            vkDestroyImageView(m_vulkan_api->getLogicDevice(), m_framebuffer.attachments[i].view, nullptr);
            vkFreeMemory(m_vulkan_api->getLogicDevice(), m_framebuffer.attachments[i].mem, nullptr);
        }
        vkDestroyFramebuffer(m_vulkan_api->getLogicDevice(), m_framebuffer.framebuffer, nullptr);

        // recreate
        setupAttachments();
        setupFramebuffer();
    }
    uint32_t PickPass::pick(const Vector2& picked_uv)
    {
        // stage0: Render ID image
        //-----------------------------------------------------------------------
        // calculate uv
        uint32_t pixel_x = static_cast<uint32_t>(picked_uv.x * m_vulkan_api->getSwapchainInfo().viewport.width +
                                                 m_vulkan_api->getSwapchainInfo().viewport.x);
        uint32_t pixel_y = static_cast<uint32_t>(picked_uv.y * m_vulkan_api->getSwapchainInfo().viewport.height +
                                                 m_vulkan_api->getSwapchainInfo().viewport.y);

        // Index from 2D image UV to 1D buffer
        uint32_t picked_pixel_index = m_vulkan_api->getSwapchainInfo().extent.width * pixel_y + pixel_x;

        // write data
        struct MeshNode
        {
            const Matrix4x4* model_matrix {nullptr};
            uint32_t         node_id;
        };

        std::map<VulkanPBRMaterial*, std::map<VulkanMesh*, std::vector<MeshNode>>> main_camera_mesh_drawcall_batch;

        // reorganize mesh
        for (RenderMeshNode& node : *(m_visiable_nodes.p_main_camera_visible_mesh_nodes))
        {
            auto& mesh_instanced = main_camera_mesh_drawcall_batch[node.ref_material];
            auto& model_nodes    = mesh_instanced[node.ref_mesh];

            MeshNode temp;
            temp.model_matrix = node.model_matrix;
            temp.node_id      = node.node_id;

            model_nodes.push_back(temp);
        }

        // prepare render command context, Update m_current_command_buffer
        m_vulkan_api->prepareContext();

        // reset storage buffer offset
        m_global_render_resource->_storage_buffer._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] =
            m_global_render_resource->_storage_buffer
                ._global_upload_ringbuffers_begin[m_vulkan_api->getCurrentFrameIndex()];

        // wait fence
        vkWaitForFences(m_vulkan_api->getLogicDevice(),
                        1,
                        &m_vulkan_api->getFenceList()[m_vulkan_api->getCurrentFrameIndex()],
                        VK_TRUE,
                        UINT64_MAX);

        // reset command buffer
        vkResetCommandBuffer(m_vulkan_api->getCurrentCommandBuffer(), 0);

        // Begin command buffer
        VkCommandBufferBeginInfo command_buffer_begin_info {};
        command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags            = 0;
        command_buffer_begin_info.pInheritanceInfo = nullptr;

        bool res_begin_command_buffer =
            vkBeginCommandBuffer(m_vulkan_api->getCurrentCommandBuffer(), &command_buffer_begin_info);

        // Image Layout Transformation
        VkImageMemoryBarrier transfer_to_render_barrier {};
        transfer_to_render_barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        transfer_to_render_barrier.pNext               = nullptr;
        transfer_to_render_barrier.srcAccessMask       = 0;
        transfer_to_render_barrier.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        transfer_to_render_barrier.oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
        transfer_to_render_barrier.newLayout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        transfer_to_render_barrier.srcQueueFamilyIndex = m_vulkan_api->getQueueFamilyIndices().graphics_family.value();
        transfer_to_render_barrier.dstQueueFamilyIndex = m_vulkan_api->getQueueFamilyIndices().graphics_family.value();
        transfer_to_render_barrier.image               = m_framebuffer.attachments[0].image;
        transfer_to_render_barrier.subresourceRange    = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
        vkCmdPipelineBarrier(m_vulkan_api->getCurrentCommandBuffer(),
                             VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &transfer_to_render_barrier);

        // Begin RenderPass
        VkRenderPassBeginInfo renderpass_begin_info {};
        renderpass_begin_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpass_begin_info.renderPass        = m_framebuffer.render_pass;
        renderpass_begin_info.framebuffer       = m_framebuffer.framebuffer;
        renderpass_begin_info.renderArea.offset = {0, 0};
        renderpass_begin_info.renderArea.extent = m_vulkan_api->getSwapchainInfo().extent;

        VkClearColorValue color_value         = {0, 0, 0, 0};
        VkClearValue      clearValues[2]      = {color_value, {1.0f, 0}};
        renderpass_begin_info.clearValueCount = 2;
        renderpass_begin_info.pClearValues    = clearValues;

        vkCmdBeginRenderPass(
            m_vulkan_api->getCurrentCommandBuffer(), &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        m_vulkan_api->pushEvent(m_vulkan_api->getCurrentCommandBuffer(), "Mesh Inefficient Pick", color);

        // Bind Pipeline
        vkCmdBindPipeline(
            m_vulkan_api->getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_render_pipelines[0].pipeline);

        // set viewport and scissor
        SwapChainDesc swap_chain_desc = m_vulkan_api->getSwapchainInfo();
        vkCmdSetViewport(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, &swap_chain_desc.viewport);
        vkCmdSetScissor(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, &swap_chain_desc.scissor);

        // perframe storage buffer
        uint32_t perframe_dynamic_offset =
            roundUp(m_global_render_resource->_storage_buffer
                        ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()],
                    m_global_render_resource->_storage_buffer._min_storage_buffer_offset_alignment);
        m_global_render_resource->_storage_buffer._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] =
            perframe_dynamic_offset + sizeof(MeshInefficientPickPerframeStorageBufferObject);
        assert(m_global_render_resource->_storage_buffer
                   ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] <=
               (m_global_render_resource->_storage_buffer
                    ._global_upload_ringbuffers_begin[m_vulkan_api->getCurrentFrameIndex()] +
                m_global_render_resource->_storage_buffer
                    ._global_upload_ringbuffers_size[m_vulkan_api->getCurrentFrameIndex()]));
        (*reinterpret_cast<MeshInefficientPickPerframeStorageBufferObject*>(
            reinterpret_cast<uintptr_t>(
                m_global_render_resource->_storage_buffer._global_upload_ringbuffer_memory_pointer) +
            perframe_dynamic_offset)) = _mesh_inefficient_pick_perframe_storage_buffer_object;

        // render
        for (auto& pair1 : main_camera_mesh_drawcall_batch) ///< Same material
        {
            VulkanPBRMaterial& material       = (*pair1.first);
            auto&              mesh_instanced = pair1.second;

            for (auto& pair2 : mesh_instanced) ///< Same mesh
            {
                VulkanMesh& mesh       = (*pair2.first);
                auto&       mesh_nodes = pair2.second;

                uint32_t total_instance_count = static_cast<uint32_t>(mesh_nodes.size()); ///< mesh count
                if (total_instance_count > 0)
                {
                    // bind vertex buffer
                    VkBuffer     vertex_buffers[] = {mesh.mesh_vertex_position_buffer};
                    VkDeviceSize offsets[]        = {0};
                    vkCmdBindVertexBuffers(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, vertex_buffers, offsets);

                    // bind indice buffer
                    vkCmdBindIndexBuffer(
                        m_vulkan_api->getCurrentCommandBuffer(), mesh.mesh_index_buffer, 0, VK_INDEX_TYPE_UINT32);

                    // Calculate the number of draw calls required for instance rendering
                    uint32_t drawcall_max_instance_count =
                        (sizeof(MeshInefficientPickPerdrawcallStorageBufferObject::model_matrices) /
                         sizeof(MeshInefficientPickPerdrawcallStorageBufferObject::model_matrices[0]));
                    uint32_t drawcall_count =
                        roundUp(total_instance_count, drawcall_max_instance_count) / drawcall_max_instance_count;
                    for (uint32_t drawcall_index = 0; drawcall_index < drawcall_count; ++drawcall_index)
                    {
                        // Calculate the number of instances to be rendered this timeS
                        uint32_t current_instance_count =
                            ((total_instance_count - drawcall_max_instance_count * drawcall_index) <
                             drawcall_max_instance_count) ?
                                (total_instance_count - drawcall_max_instance_count * drawcall_index) :
                                drawcall_max_instance_count;

                        // perdrawcall storage buffer
                        uint32_t perdrawcall_dynamic_offset =
                            roundUp(m_global_render_resource->_storage_buffer
                                        ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()],
                                    m_global_render_resource->_storage_buffer._min_storage_buffer_offset_alignment);
                        m_global_render_resource->_storage_buffer
                            ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] =
                            perdrawcall_dynamic_offset + sizeof(MeshInefficientPickPerdrawcallStorageBufferObject);
                        assert(m_global_render_resource->_storage_buffer
                                   ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] <=
                               (m_global_render_resource->_storage_buffer
                                    ._global_upload_ringbuffers_begin[m_vulkan_api->getCurrentFrameIndex()] +
                                m_global_render_resource->_storage_buffer
                                    ._global_upload_ringbuffers_size[m_vulkan_api->getCurrentFrameIndex()]));

                        MeshInefficientPickPerdrawcallStorageBufferObject& perdrawcall_storage_buffer_object =
                            (*reinterpret_cast<MeshInefficientPickPerdrawcallStorageBufferObject*>(
                                reinterpret_cast<uintptr_t>(m_global_render_resource->_storage_buffer
                                                                ._global_upload_ringbuffer_memory_pointer) +
                                perdrawcall_dynamic_offset));
                        for (uint32_t i = 0; i < current_instance_count; ++i)
                        {
                            perdrawcall_storage_buffer_object.model_matrices[i] =
                                *mesh_nodes[drawcall_max_instance_count * drawcall_index + i].model_matrix;
                            perdrawcall_storage_buffer_object.node_ids[i] =
                                mesh_nodes[drawcall_max_instance_count * drawcall_index + i].node_id;
                        }

                        // bind perdrawcall
                        uint32_t dynamic_offsets[2] = {perframe_dynamic_offset, perdrawcall_dynamic_offset};
                        vkCmdBindDescriptorSets(m_vulkan_api->getCurrentCommandBuffer(),
                                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                m_render_pipelines[0].layout,
                                                0,
                                                1,
                                                &m_descriptor_infos[0].descriptor_set,
                                                sizeof(dynamic_offsets) / sizeof(dynamic_offsets[0]),
                                                dynamic_offsets);

                        // drawcall
                        vkCmdDrawIndexed(m_vulkan_api->getCurrentCommandBuffer(),
                                         mesh.mesh_index_count,
                                         current_instance_count,
                                         0,
                                         0,
                                         0);
                    }
                }
            }
        }

        m_vulkan_api->popEvent(m_vulkan_api->getCurrentCommandBuffer());
        // end render pass
        vkCmdEndRenderPass(m_vulkan_api->getCurrentCommandBuffer());

        // end command buffer
        vkEndCommandBuffer(m_vulkan_api->getCurrentCommandBuffer());

        // reset fence
        vkResetFences(
            m_vulkan_api->getLogicDevice(), 1, &m_vulkan_api->getFenceList()[m_vulkan_api->getCurrentFrameIndex()]);

        // submit
        VkSubmitInfo submit_info         = {};
        submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount   = 0;
        submit_info.pWaitSemaphores      = NULL;
        submit_info.pWaitDstStageMask    = 0;
        submit_info.commandBufferCount   = 1;
        VkCommandBuffer commandBuffer    = m_vulkan_api->getCurrentCommandBuffer();
        submit_info.pCommandBuffers      = &commandBuffer;
        submit_info.signalSemaphoreCount = 0;
        submit_info.pSignalSemaphores    = NULL;

        bool res_queue_submit = vkQueueSubmit(m_vulkan_api->getGraphicsQueue(),
                                              1,
                                              &submit_info,
                                              m_vulkan_api->getFenceList()[m_vulkan_api->getCurrentFrameIndex()]);

        // update frame index
        m_vulkan_api->setCurrentFrameIndex((m_vulkan_api->getCurrentFrameIndex() + 1) %
                                           m_vulkan_api->getMaxFramesInFlight());

        // stage1: Calculate the submesh ID of the UV
        //-----------------------------------------------------------------------
        // wait fence
        vkWaitForFences(m_vulkan_api->getLogicDevice(),
                        1,
                        &m_vulkan_api->getFenceList()[m_vulkan_api->getCurrentFrameIndex()],
                        VK_TRUE,
                        UINT64_MAX);

        // begin command buffer
        auto command_buffer = m_vulkan_api->beginSingleTimeCommands();

        // create staging buffer
        uint32_t buffer_size =
            m_vulkan_api->getSwapchainInfo().extent.width * m_vulkan_api->getSwapchainInfo().extent.height * 4;
        VkBuffer       inefficient_staging_buffer;
        VkDeviceMemory inefficient_staging_buffer_memory;
        m_vulkan_api->createBuffer(buffer_size,
                                   VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                   inefficient_staging_buffer,
                                   inefficient_staging_buffer_memory);

        // Image Layout Transformation
        VkImageMemoryBarrier copy_to_buffer_barrier {};
        copy_to_buffer_barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        copy_to_buffer_barrier.pNext               = nullptr;
        copy_to_buffer_barrier.srcAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        copy_to_buffer_barrier.dstAccessMask       = VK_ACCESS_TRANSFER_READ_BIT;
        copy_to_buffer_barrier.oldLayout           = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        copy_to_buffer_barrier.newLayout           = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        copy_to_buffer_barrier.srcQueueFamilyIndex = m_vulkan_api->getQueueFamilyIndices().graphics_family.value();
        copy_to_buffer_barrier.dstQueueFamilyIndex = m_vulkan_api->getQueueFamilyIndices().graphics_family.value();
        copy_to_buffer_barrier.image               = m_framebuffer.attachments[0].image;
        copy_to_buffer_barrier.subresourceRange    = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
        vkCmdPipelineBarrier(command_buffer,
                             VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &copy_to_buffer_barrier);

        // image copy buffer
        VkBufferImageCopy region {};
        region.bufferOffset                    = 0;
        region.bufferRowLength                 = 0;
        region.bufferImageHeight               = 0;
        region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel       = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount     = 1;
        region.imageOffset                     = {0, 0, 0};
        region.imageExtent                     = {
            m_vulkan_api->getSwapchainInfo().extent.width, m_vulkan_api->getSwapchainInfo().extent.height, 1};
        vkCmdCopyImageToBuffer(command_buffer,
                               m_framebuffer.attachments[0].image,
                               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               inefficient_staging_buffer,
                               1,
                               &region);

        // end command buffer
        m_vulkan_api->endSingleTimeCommands(command_buffer);

        // read guid data
        uint32_t* data = nullptr;
        vkMapMemory(
            m_vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory, 0, buffer_size, 0, (void**)&data);
        uint32_t node_id = data[picked_pixel_index];
        vkUnmapMemory(m_vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory);

        // destory staging
        vkDestroyBuffer(m_vulkan_api->getLogicDevice(), inefficient_staging_buffer, nullptr);
        vkFreeMemory(m_vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory, nullptr);

        return node_id;
    }
} // namespace VKernel

#include "runtime/function/render/passes/main_camera_pass.h"

#include "runtime/function/render/render_helper.h"
#include "runtime/function/render/render_mesh.h"
#include "runtime/function/render/render_resource.h"

#include <mesh_frag.h>
#include <mesh_vert.h>
#include <skybox_frag.h>
#include <skybox_vert.h>

#include <map>
#include <stdexcept>

namespace VKernel
{
    void MainCameraPass::initialize(const RenderPassInitInfo* init_info)
    {
        // init basic class
        RenderPass::initialize(init_info);

        // init
        setupAttachments();
        setupRenderPass();
        setupDescriptorSetLayout();
        setupPipelines();
        setupDescriptorSet();
        setupSwapchainFramebuffers();
    }

    void MainCameraPass::preparePassData(std::shared_ptr<RenderResourceBase> render_resource)
    {
        const RenderResource* vulkan_resource = static_cast<const RenderResource*>(render_resource.get());
        if (vulkan_resource)
        {
            m_mesh_perframe_storage_buffer_object = vulkan_resource->m_mesh_perframe_storage_buffer_object;
        }
    }

    void
    MainCameraPass::drawForward(UIPass& ui_pass, CombineUIPass& combine_ui_pass, uint32_t current_swapchain_image_index)
    {
        // Begin RenderPass
        {
            // begin renderpass info
            VkRenderPassBeginInfo renderpass_begin_info {};
            renderpass_begin_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderpass_begin_info.renderPass        = m_framebuffer.render_pass;
            renderpass_begin_info.framebuffer       = m_swapchain_framebuffers[current_swapchain_image_index];
            renderpass_begin_info.renderArea.offset = {0, 0};
            renderpass_begin_info.renderArea.extent = m_vulkan_api->getSwapchainInfo().extent;

            // clear value
            VkClearValue clear_values[_main_camera_pass_attachment_count];
            clear_values[_main_camera_pass_swap_chain_image].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
            clear_values[_main_camera_pass_depth].depthStencil     = {1.0f, 0};
            renderpass_begin_info.clearValueCount                  = (sizeof(clear_values) / sizeof(clear_values[0]));
            renderpass_begin_info.pClearValues                     = clear_values;

            // begin renderpass
            vkCmdBeginRenderPass(
                m_vulkan_api->getCurrentCommandBuffer(), &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
        }

        // draw
        drawMeshLighting();

        drawSkybox();

        vkCmdNextSubpass(m_vulkan_api->getCurrentCommandBuffer(), VK_SUBPASS_CONTENTS_INLINE);

        ui_pass.draw();

        vkCmdNextSubpass(m_vulkan_api->getCurrentCommandBuffer(), VK_SUBPASS_CONTENTS_INLINE);

        combine_ui_pass.draw();

        // end renderpass
        vkCmdEndRenderPass(m_vulkan_api->getCurrentCommandBuffer());
    }

    void MainCameraPass::updateAfterFramebufferRecreate()
    {
        // destory iamge
        for (size_t i = 0; i < m_framebuffer.attachments.size(); i++)
        {
            vkDestroyImage(m_vulkan_api->getLogicDevice(), m_framebuffer.attachments[i].image, nullptr);
            vkDestroyImageView(m_vulkan_api->getLogicDevice(), m_framebuffer.attachments[i].view, nullptr);
            vkFreeMemory(m_vulkan_api->getLogicDevice(), m_framebuffer.attachments[i].mem, nullptr);
        }

        // destory framebuffer
        for (auto framebuffer : m_swapchain_framebuffers)
        {
            vkDestroyFramebuffer(m_vulkan_api->getLogicDevice(), framebuffer, nullptr);
        }

        // recreate
        setupAttachments();

        setupSwapchainFramebuffers();
    }

    void MainCameraPass::setupAttachments()
    {
        m_framebuffer.attachments.resize(_main_camera_pass_custom_attachment_count); ///< resize vector size

        // set attachment format
        m_framebuffer.attachments[_main_camera_pass_backup_buffer_odd].format  = VK_FORMAT_R16G16B16A16_SFLOAT;
        m_framebuffer.attachments[_main_camera_pass_backup_buffer_even].format = VK_FORMAT_R16G16B16A16_SFLOAT;

        // Number of Attachments
        for (int buffer_index = 0; buffer_index < _main_camera_pass_custom_attachment_count; ++buffer_index)
        {

            // create image
            m_vulkan_api->createImage(m_vulkan_api->getSwapchainInfo().extent.width,
                                      m_vulkan_api->getSwapchainInfo().extent.height,
                                      m_framebuffer.attachments[buffer_index].format,
                                      VK_IMAGE_TILING_OPTIMAL,
                                      VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                          VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
                                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                      m_framebuffer.attachments[buffer_index].image,
                                      m_framebuffer.attachments[buffer_index].mem,
                                      0,
                                      1,
                                      1);

            // create image view
            m_vulkan_api->createImageView(m_framebuffer.attachments[buffer_index].image,
                                          m_framebuffer.attachments[buffer_index].format,
                                          VK_IMAGE_ASPECT_COLOR_BIT,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          1,
                                          1,
                                          m_framebuffer.attachments[buffer_index].view);
        }
    }

    void MainCameraPass::setupRenderPass()
    {
        // Attachment Description
        VkAttachmentDescription attachments[_main_camera_pass_attachment_count] = {};

        VkAttachmentDescription& backup_odd_color_attachment_description =
            attachments[_main_camera_pass_backup_buffer_odd];
        backup_odd_color_attachment_description.format =
            m_framebuffer.attachments[_main_camera_pass_backup_buffer_odd].format;
        backup_odd_color_attachment_description.samples        = VK_SAMPLE_COUNT_1_BIT;
        backup_odd_color_attachment_description.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        backup_odd_color_attachment_description.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        backup_odd_color_attachment_description.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        backup_odd_color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        backup_odd_color_attachment_description.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        backup_odd_color_attachment_description.finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentDescription& backup_even_color_attachment_description =
            attachments[_main_camera_pass_backup_buffer_even];
        backup_even_color_attachment_description.format =
            m_framebuffer.attachments[_main_camera_pass_backup_buffer_even].format;
        backup_even_color_attachment_description.samples        = VK_SAMPLE_COUNT_1_BIT;
        backup_even_color_attachment_description.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        backup_even_color_attachment_description.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        backup_even_color_attachment_description.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        backup_even_color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        backup_even_color_attachment_description.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        backup_even_color_attachment_description.finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentDescription& depth_attachment_description = attachments[_main_camera_pass_depth];
        depth_attachment_description.format                   = m_vulkan_api->getDepthImageInfo().depth_image_format;
        depth_attachment_description.samples                  = VK_SAMPLE_COUNT_1_BIT;
        depth_attachment_description.loadOp                   = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depth_attachment_description.storeOp                  = VK_ATTACHMENT_STORE_OP_STORE;
        depth_attachment_description.stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment_description.stencilStoreOp           = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment_description.initialLayout            = VK_IMAGE_LAYOUT_UNDEFINED;
        depth_attachment_description.finalLayout              = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription& swapchain_image_attachment_description =
            attachments[_main_camera_pass_swap_chain_image];
        swapchain_image_attachment_description.format         = m_vulkan_api->getSwapchainInfo().image_format;
        swapchain_image_attachment_description.samples        = VK_SAMPLE_COUNT_1_BIT;
        swapchain_image_attachment_description.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        swapchain_image_attachment_description.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        swapchain_image_attachment_description.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        swapchain_image_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        swapchain_image_attachment_description.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        swapchain_image_attachment_description.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // Attachment Reference and subpass description
        VkSubpassDescription subpasses[_main_camera_subpass_count] = {};

        VkAttachmentReference forward_lighting_pass_color_attachments_reference[1] = {}; ///< forward subpass
        forward_lighting_pass_color_attachments_reference[0].attachment =
            &backup_odd_color_attachment_description - attachments; ///< index
        forward_lighting_pass_color_attachments_reference[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference forward_lighting_pass_depth_attachment_reference {};
        forward_lighting_pass_depth_attachment_reference.attachment = &depth_attachment_description - attachments;
        forward_lighting_pass_depth_attachment_reference.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription& forward_lighting_pass = subpasses[_main_camera_subpass_forward_lighting];
        forward_lighting_pass.pipelineBindPoint     = VK_PIPELINE_BIND_POINT_GRAPHICS;
        forward_lighting_pass.inputAttachmentCount  = 0U;
        forward_lighting_pass.pInputAttachments     = NULL;
        forward_lighting_pass.colorAttachmentCount  = sizeof(forward_lighting_pass_color_attachments_reference) /
                                                     sizeof(forward_lighting_pass_color_attachments_reference[0]);
        forward_lighting_pass.pColorAttachments       = &forward_lighting_pass_color_attachments_reference[0];
        forward_lighting_pass.pDepthStencilAttachment = &forward_lighting_pass_depth_attachment_reference;
        forward_lighting_pass.preserveAttachmentCount = 0;
        forward_lighting_pass.pPreserveAttachments    = NULL;

        VkAttachmentReference ui_pass_color_attachment_reference {}; ///< ui subpass
        ui_pass_color_attachment_reference.attachment = &backup_even_color_attachment_description - attachments;
        ui_pass_color_attachment_reference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription& ui_pass   = subpasses[_main_camera_subpass_ui];
        ui_pass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
        ui_pass.inputAttachmentCount    = 0;
        ui_pass.pInputAttachments       = NULL;
        ui_pass.colorAttachmentCount    = 1;
        ui_pass.pColorAttachments       = &ui_pass_color_attachment_reference;
        ui_pass.pDepthStencilAttachment = NULL;
        ui_pass.preserveAttachmentCount = 0;
        ui_pass.pPreserveAttachments    = nullptr;

        VkAttachmentReference combine_ui_pass_input_attachments_reference[2] = {};
        combine_ui_pass_input_attachments_reference[0].attachment =
            &backup_odd_color_attachment_description - attachments;
        combine_ui_pass_input_attachments_reference[0].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        combine_ui_pass_input_attachments_reference[1].attachment =
            &backup_even_color_attachment_description - attachments;
        combine_ui_pass_input_attachments_reference[1].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentReference combine_ui_pass_color_attachment_reference {};
        combine_ui_pass_color_attachment_reference.attachment = &swapchain_image_attachment_description - attachments;
        combine_ui_pass_color_attachment_reference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription& combine_ui_pass = subpasses[_main_camera_subpass_combine_ui];
        combine_ui_pass.pipelineBindPoint     = VK_PIPELINE_BIND_POINT_GRAPHICS;
        combine_ui_pass.inputAttachmentCount =
            sizeof(combine_ui_pass_input_attachments_reference) /
            sizeof(combine_ui_pass_input_attachments_reference[0]); ///< input accachment
        combine_ui_pass.pInputAttachments       = combine_ui_pass_input_attachments_reference;
        combine_ui_pass.colorAttachmentCount    = 1;
        combine_ui_pass.pColorAttachments       = &combine_ui_pass_color_attachment_reference;
        combine_ui_pass.pDepthStencilAttachment = NULL;
        combine_ui_pass.preserveAttachmentCount = 0;
        combine_ui_pass.pPreserveAttachments    = NULL;

        // Subpass Dependency
        VkSubpassDependency dependencies[3] = {};

        VkSubpassDependency& forward_lighting_pass_depend_on_deferred_lighting_pass = dependencies[0];
        forward_lighting_pass_depend_on_deferred_lighting_pass.srcSubpass           = VK_SUBPASS_EXTERNAL;
        forward_lighting_pass_depend_on_deferred_lighting_pass.dstSubpass = _main_camera_subpass_forward_lighting;
        forward_lighting_pass_depend_on_deferred_lighting_pass.srcStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        forward_lighting_pass_depend_on_deferred_lighting_pass.dstStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        forward_lighting_pass_depend_on_deferred_lighting_pass.srcAccessMask   = 0;
        forward_lighting_pass_depend_on_deferred_lighting_pass.dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        forward_lighting_pass_depend_on_deferred_lighting_pass.dependencyFlags = 0;

        VkSubpassDependency& ui_pass_depend_on_fxaa_pass = dependencies[1];
        ui_pass_depend_on_fxaa_pass.srcSubpass           = _main_camera_subpass_forward_lighting;
        ui_pass_depend_on_fxaa_pass.dstSubpass           = _main_camera_subpass_ui;
        ui_pass_depend_on_fxaa_pass.srcStageMask =
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        ui_pass_depend_on_fxaa_pass.dstStageMask =
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        ui_pass_depend_on_fxaa_pass.srcAccessMask   = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        ui_pass_depend_on_fxaa_pass.dstAccessMask   = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        ui_pass_depend_on_fxaa_pass.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        VkSubpassDependency& combine_ui_pass_depend_on_ui_pass = dependencies[2];
        combine_ui_pass_depend_on_ui_pass.srcSubpass           = _main_camera_subpass_ui;
        combine_ui_pass_depend_on_ui_pass.dstSubpass           = _main_camera_subpass_combine_ui;
        combine_ui_pass_depend_on_ui_pass.srcStageMask =
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        combine_ui_pass_depend_on_ui_pass.dstStageMask =
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        combine_ui_pass_depend_on_ui_pass.srcAccessMask =
            VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        combine_ui_pass_depend_on_ui_pass.dstAccessMask =
            VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        combine_ui_pass_depend_on_ui_pass.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        // RenderPass
        VkRenderPassCreateInfo renderpass_create_info {};
        renderpass_create_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderpass_create_info.attachmentCount = (sizeof(attachments) / sizeof(attachments[0]));
        renderpass_create_info.pAttachments    = attachments;
        renderpass_create_info.subpassCount    = (sizeof(subpasses) / sizeof(subpasses[0]));
        renderpass_create_info.pSubpasses      = subpasses;
        renderpass_create_info.dependencyCount = (sizeof(dependencies) / sizeof(dependencies[0]));
        renderpass_create_info.pDependencies   = dependencies;

        if (vkCreateRenderPass(
                m_vulkan_api->getLogicDevice(), &renderpass_create_info, nullptr, &(m_framebuffer.render_pass)) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to create render pass");
        }
    }

    void MainCameraPass::setupDescriptorSetLayout()
    {
        // DescriptorSetLayout
        m_descriptor_infos.resize(_layout_type_count);

        // _mesh_global
        {
            VkDescriptorSetLayoutBinding mesh_global_layout_bindings[5];

            VkDescriptorSetLayoutBinding& mesh_global_layout_perframe_storage_buffer_binding =
                mesh_global_layout_bindings[0];
            mesh_global_layout_perframe_storage_buffer_binding.binding = 0;
            mesh_global_layout_perframe_storage_buffer_binding.descriptorType =
                VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            mesh_global_layout_perframe_storage_buffer_binding.descriptorCount = 1;
            mesh_global_layout_perframe_storage_buffer_binding.stageFlags =
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; ///< shader stage
            mesh_global_layout_perframe_storage_buffer_binding.pImmutableSamplers = NULL;

            VkDescriptorSetLayoutBinding& mesh_global_layout_perdrawcall_storage_buffer_binding =
                mesh_global_layout_bindings[1];
            mesh_global_layout_perdrawcall_storage_buffer_binding.binding = 1;
            mesh_global_layout_perdrawcall_storage_buffer_binding.descriptorType =
                VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            mesh_global_layout_perdrawcall_storage_buffer_binding.descriptorCount    = 1;
            mesh_global_layout_perdrawcall_storage_buffer_binding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
            mesh_global_layout_perdrawcall_storage_buffer_binding.pImmutableSamplers = NULL;

            VkDescriptorSetLayoutBinding& mesh_global_layout_brdfLUT_texture_binding = mesh_global_layout_bindings[2];
            mesh_global_layout_brdfLUT_texture_binding.binding                       = 2;
            mesh_global_layout_brdfLUT_texture_binding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            mesh_global_layout_brdfLUT_texture_binding.descriptorCount    = 1;
            mesh_global_layout_brdfLUT_texture_binding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
            mesh_global_layout_brdfLUT_texture_binding.pImmutableSamplers = NULL;

            VkDescriptorSetLayoutBinding& mesh_global_layout_irradiance_texture_binding =
                mesh_global_layout_bindings[3];
            mesh_global_layout_irradiance_texture_binding         = mesh_global_layout_brdfLUT_texture_binding;
            mesh_global_layout_irradiance_texture_binding.binding = 3;

            VkDescriptorSetLayoutBinding& mesh_global_layout_specular_texture_binding = mesh_global_layout_bindings[4];
            mesh_global_layout_specular_texture_binding         = mesh_global_layout_brdfLUT_texture_binding;
            mesh_global_layout_specular_texture_binding.binding = 4;

            VkDescriptorSetLayoutCreateInfo mesh_global_layout_create_info;
            mesh_global_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            mesh_global_layout_create_info.pNext = NULL;
            mesh_global_layout_create_info.flags = 0;
            mesh_global_layout_create_info.bindingCount =
                (sizeof(mesh_global_layout_bindings) / sizeof(mesh_global_layout_bindings[0]));
            mesh_global_layout_create_info.pBindings = mesh_global_layout_bindings;

            if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_api->getLogicDevice(),
                                                          &mesh_global_layout_create_info,
                                                          nullptr,
                                                          &m_descriptor_infos[_mesh_global].layout))
            {
                throw std::runtime_error("create mesh global layout");
            }
        }

        // _mesh_per_material
        {
            VkDescriptorSetLayoutBinding mesh_material_layout_bindings[2];

            VkDescriptorSetLayoutBinding& mesh_material_layout_base_color_texture_binding =
                mesh_material_layout_bindings[0];
            mesh_material_layout_base_color_texture_binding.binding         = 0;
            mesh_material_layout_base_color_texture_binding.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            mesh_material_layout_base_color_texture_binding.descriptorCount = 1;
            mesh_material_layout_base_color_texture_binding.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
            mesh_material_layout_base_color_texture_binding.pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutBinding& mesh_material_layout_normal_roughness_texture_binding =
                mesh_material_layout_bindings[1];
            mesh_material_layout_normal_roughness_texture_binding = mesh_material_layout_base_color_texture_binding;
            mesh_material_layout_normal_roughness_texture_binding.binding = 1;

            VkDescriptorSetLayoutCreateInfo mesh_material_layout_create_info;
            mesh_material_layout_create_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            mesh_material_layout_create_info.pNext        = NULL;
            mesh_material_layout_create_info.flags        = 0;
            mesh_material_layout_create_info.bindingCount = 2;
            mesh_material_layout_create_info.pBindings    = mesh_material_layout_bindings;

            if (vkCreateDescriptorSetLayout(m_vulkan_api->getLogicDevice(),
                                            &mesh_material_layout_create_info,
                                            nullptr,
                                            &m_descriptor_infos[_mesh_per_material].layout) != VK_SUCCESS)
            {
                throw std::runtime_error("create mesh material layout");
            }
        }

        // _skybox
        {
            VkDescriptorSetLayoutBinding skybox_layout_bindings[2];

            VkDescriptorSetLayoutBinding& skybox_layout_perframe_storage_buffer_binding = skybox_layout_bindings[0];
            skybox_layout_perframe_storage_buffer_binding.binding                       = 0;
            skybox_layout_perframe_storage_buffer_binding.descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            skybox_layout_perframe_storage_buffer_binding.descriptorCount = 1;
            skybox_layout_perframe_storage_buffer_binding.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;
            skybox_layout_perframe_storage_buffer_binding.pImmutableSamplers = NULL;

            VkDescriptorSetLayoutBinding& skybox_layout_specular_texture_binding = skybox_layout_bindings[1];
            skybox_layout_specular_texture_binding.binding                       = 1;
            skybox_layout_specular_texture_binding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            skybox_layout_specular_texture_binding.descriptorCount    = 1;
            skybox_layout_specular_texture_binding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
            skybox_layout_specular_texture_binding.pImmutableSamplers = NULL;

            VkDescriptorSetLayoutCreateInfo skybox_layout_create_info {};
            skybox_layout_create_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            skybox_layout_create_info.bindingCount = 2;
            skybox_layout_create_info.pBindings    = skybox_layout_bindings;

            if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_api->getLogicDevice(),
                                                          &skybox_layout_create_info,
                                                          nullptr,
                                                          &m_descriptor_infos[_skybox].layout))
            {
                throw std::runtime_error("create skybox layout");
            }
        }
    }

    void MainCameraPass::setupPipelines()
    {

        m_render_pipelines.resize(_render_pipeline_type_count);

        // forward render
        {
            // Pipeline Layout
            VkDescriptorSetLayout      descriptorset_layouts[2] = {m_descriptor_infos[_mesh_global].layout,
                                                                   m_descriptor_infos[_mesh_per_material].layout};
            VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
            pipeline_layout_create_info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipeline_layout_create_info.setLayoutCount = 2;
            pipeline_layout_create_info.pSetLayouts    = descriptorset_layouts;

            if (vkCreatePipelineLayout(m_vulkan_api->getLogicDevice(),
                                       &pipeline_layout_create_info,
                                       nullptr,
                                       &(m_render_pipelines[_render_pipeline_type_mesh_lighting].layout)) != VK_SUCCESS)
            {
                throw std::runtime_error("create mesh lighting pipeline layout");
            }

            // Shader Module
            VkShaderModule vert_shader_module = m_vulkan_api->createShaderModule(MESH_VERT);
            VkShaderModule frag_shader_module = m_vulkan_api->createShaderModule(MESH_FRAG);

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
            vertex_input_state_create_info.vertexBindingDescriptionCount   = vertex_binding_descriptions.size();
            vertex_input_state_create_info.pVertexBindingDescriptions      = &vertex_binding_descriptions[0];
            vertex_input_state_create_info.vertexAttributeDescriptionCount = vertex_attribute_descriptions.size();
            vertex_input_state_create_info.pVertexAttributeDescriptions    = &vertex_attribute_descriptions[0];

            // Assembly State
            VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info {};
            input_assembly_create_info.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; ///< triangle
            input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

            // Viewport State
            VkPipelineViewportStateCreateInfo viewport_state_create_info {};
            viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewport_state_create_info.viewportCount = 1;
            viewport_state_create_info.scissorCount  = 1;

            // Rasterization State
            VkPipelineRasterizationStateCreateInfo rasterization_state_create_info {};
            rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterization_state_create_info.depthClampEnable        = VK_FALSE;
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
            multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisample_state_create_info.sampleShadingEnable  = VK_FALSE;
            multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

            // ColorBlendAttachment State
            VkPipelineColorBlendAttachmentState color_blend_attachments[1] = {};
            color_blend_attachments[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            color_blend_attachments[0].blendEnable = VK_FALSE;

            // ColorBlend State
            VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {};
            color_blend_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            color_blend_state_create_info.logicOpEnable = VK_FALSE;
            color_blend_state_create_info.logicOp       = VK_LOGIC_OP_COPY;
            color_blend_state_create_info.attachmentCount =
                sizeof(color_blend_attachments) / sizeof(color_blend_attachments[0]);
            color_blend_state_create_info.pAttachments      = &color_blend_attachments[0];
            color_blend_state_create_info.blendConstants[0] = 0.0f;
            color_blend_state_create_info.blendConstants[1] = 0.0f;
            color_blend_state_create_info.blendConstants[2] = 0.0f;
            color_blend_state_create_info.blendConstants[3] = 0.0f;

            // depth state
            VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info {};
            depth_stencil_create_info.sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depth_stencil_create_info.depthTestEnable  = VK_TRUE;
            depth_stencil_create_info.depthWriteEnable = VK_TRUE;
            depth_stencil_create_info.depthCompareOp   = VK_COMPARE_OP_LESS;
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
            pipelineInfo.layout              = m_render_pipelines[_render_pipeline_type_mesh_lighting].layout;
            pipelineInfo.renderPass          = m_framebuffer.render_pass;
            pipelineInfo.subpass             = _main_camera_subpass_forward_lighting;
            pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;

            // create Pipeline
            if (vkCreateGraphicsPipelines(m_vulkan_api->getLogicDevice(),
                                          VK_NULL_HANDLE,
                                          1,
                                          &pipelineInfo,
                                          nullptr,
                                          &(m_render_pipelines[_render_pipeline_type_mesh_lighting].pipeline)) !=
                VK_SUCCESS)
            {
                throw std::runtime_error("create mesh lighting graphics pipeline");
            }

            // destory shader module
            m_vulkan_api->destroyShaderModule(vert_shader_module);
            m_vulkan_api->destroyShaderModule(frag_shader_module);
        }

        // skybox
        {
            // Pipeline Layout
            VkDescriptorSetLayout      descriptorset_layouts[1] = {m_descriptor_infos[_skybox].layout};
            VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
            pipeline_layout_create_info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipeline_layout_create_info.setLayoutCount = 1;
            pipeline_layout_create_info.pSetLayouts    = descriptorset_layouts;

            if (vkCreatePipelineLayout(m_vulkan_api->getLogicDevice(),
                                       &pipeline_layout_create_info,
                                       nullptr,
                                       &m_render_pipelines[_render_pipeline_type_skybox].layout) != VK_SUCCESS)
            {
                throw std::runtime_error("create skybox pipeline layout");
            }

            // Shader Module
            VkShaderModule vert_shader_module = m_vulkan_api->createShaderModule(SKYBOX_VERT);
            VkShaderModule frag_shader_module = m_vulkan_api->createShaderModule(SKYBOX_FRAG);

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
            vertex_input_state_create_info.vertexBindingDescriptionCount   = 0;
            vertex_input_state_create_info.pVertexBindingDescriptions      = NULL;
            vertex_input_state_create_info.vertexAttributeDescriptionCount = 0;
            vertex_input_state_create_info.pVertexAttributeDescriptions    = NULL;

            // Assembly State
            VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info {};
            input_assembly_create_info.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

            // Viewport State
            VkPipelineViewportStateCreateInfo viewport_state_create_info {};
            viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewport_state_create_info.viewportCount = 1;
            viewport_state_create_info.scissorCount  = 1;

            // Rasterization State
            VkPipelineRasterizationStateCreateInfo rasterization_state_create_info {};
            rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterization_state_create_info.depthClampEnable        = VK_FALSE;
            rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
            rasterization_state_create_info.polygonMode             = VK_POLYGON_MODE_FILL;
            rasterization_state_create_info.lineWidth               = 1.0f;
            rasterization_state_create_info.cullMode                = VK_CULL_MODE_BACK_BIT;
            rasterization_state_create_info.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            rasterization_state_create_info.depthBiasEnable         = VK_FALSE;
            rasterization_state_create_info.depthBiasConstantFactor = 0.0f;
            rasterization_state_create_info.depthBiasClamp          = 0.0f;
            rasterization_state_create_info.depthBiasSlopeFactor    = 0.0f;

            // Multisample State
            VkPipelineMultisampleStateCreateInfo multisample_state_create_info {};
            multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisample_state_create_info.sampleShadingEnable  = VK_FALSE;
            multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

            // ColorBlendAttachment State
            VkPipelineColorBlendAttachmentState color_blend_attachments[1] = {};
            color_blend_attachments[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            color_blend_attachments[0].blendEnable         = VK_FALSE;
            color_blend_attachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
            color_blend_attachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
            color_blend_attachments[0].colorBlendOp        = VK_BLEND_OP_ADD;
            color_blend_attachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            color_blend_attachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            color_blend_attachments[0].alphaBlendOp        = VK_BLEND_OP_ADD;

            VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {};
            color_blend_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            color_blend_state_create_info.logicOpEnable = VK_FALSE;
            color_blend_state_create_info.logicOp       = VK_LOGIC_OP_COPY;
            color_blend_state_create_info.attachmentCount =
                sizeof(color_blend_attachments) / sizeof(color_blend_attachments[0]);
            color_blend_state_create_info.pAttachments      = &color_blend_attachments[0];
            color_blend_state_create_info.blendConstants[0] = 0.0f;
            color_blend_state_create_info.blendConstants[1] = 0.0f;
            color_blend_state_create_info.blendConstants[2] = 0.0f;
            color_blend_state_create_info.blendConstants[3] = 0.0f;

            // depth state
            VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info {};
            depth_stencil_create_info.sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depth_stencil_create_info.depthTestEnable  = VK_TRUE;
            depth_stencil_create_info.depthWriteEnable = VK_TRUE;
            depth_stencil_create_info.depthCompareOp   = VK_COMPARE_OP_LESS;
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
            pipelineInfo.layout              = m_render_pipelines[_render_pipeline_type_skybox].layout;
            pipelineInfo.renderPass          = m_framebuffer.render_pass;
            pipelineInfo.subpass             = _main_camera_subpass_forward_lighting;
            pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;

            // create Pipeline
            if (VK_SUCCESS != vkCreateGraphicsPipelines(m_vulkan_api->getLogicDevice(),
                                                        VK_NULL_HANDLE,
                                                        1,
                                                        &pipelineInfo,
                                                        nullptr,
                                                        &m_render_pipelines[_render_pipeline_type_skybox].pipeline))
            {
                throw std::runtime_error("create skybox graphics pipeline");
            }

            // destory shader module
            m_vulkan_api->destroyShaderModule(vert_shader_module);
            m_vulkan_api->destroyShaderModule(frag_shader_module);
        }
    }

    void VKernel::MainCameraPass::setupDescriptorSet()
    {
        setupModelGlobalDescriptorSet();
        setupSkyboxDescriptorSet();
    }

    void MainCameraPass::setupSwapchainFramebuffers()
    {

        m_swapchain_framebuffers.resize(m_vulkan_api->getSwapchainInfo().imageViews.size());

        // create frame buffer for every imageview
        for (size_t i = 0; i < m_vulkan_api->getSwapchainInfo().imageViews.size(); i++)
        {
            // image view array
            VkImageView framebuffer_attachments_for_image_view[_main_camera_pass_attachment_count] = {
                m_framebuffer.attachments[_main_camera_pass_backup_buffer_odd].view,
                m_framebuffer.attachments[_main_camera_pass_backup_buffer_even].view,
                m_vulkan_api->getDepthImageInfo().depth_image_view,
                m_vulkan_api->getSwapchainInfo().imageViews[i]};

            VkFramebufferCreateInfo framebuffer_create_info {};
            framebuffer_create_info.sType      = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_create_info.flags      = 0U;
            framebuffer_create_info.renderPass = m_framebuffer.render_pass;
            framebuffer_create_info.attachmentCount =
                (sizeof(framebuffer_attachments_for_image_view) / sizeof(framebuffer_attachments_for_image_view[0]));
            framebuffer_create_info.pAttachments = framebuffer_attachments_for_image_view;
            framebuffer_create_info.width        = m_vulkan_api->getSwapchainInfo().extent.width;
            framebuffer_create_info.height       = m_vulkan_api->getSwapchainInfo().extent.height;
            framebuffer_create_info.layers       = 1;

            if (VK_SUCCESS !=
                vkCreateFramebuffer(
                    m_vulkan_api->getLogicDevice(), &framebuffer_create_info, nullptr, &m_swapchain_framebuffers[i]))
            {
                throw std::runtime_error("create main camera framebuffer");
            }
        }
    }

    void MainCameraPass::setupModelGlobalDescriptorSet()
    {
        // DescriptorSet
        VkDescriptorSetAllocateInfo mesh_global_descriptor_set_alloc_info;
        mesh_global_descriptor_set_alloc_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        mesh_global_descriptor_set_alloc_info.pNext              = NULL;
        mesh_global_descriptor_set_alloc_info.descriptorPool     = m_vulkan_api->getDescriptorPool();
        mesh_global_descriptor_set_alloc_info.descriptorSetCount = 1;
        mesh_global_descriptor_set_alloc_info.pSetLayouts        = &m_descriptor_infos[_mesh_global].layout;

        if (VK_SUCCESS != vkAllocateDescriptorSets(m_vulkan_api->getLogicDevice(),
                                                   &mesh_global_descriptor_set_alloc_info,
                                                   &m_descriptor_infos[_mesh_global].descriptor_set))
        {
            throw std::runtime_error("allocate mesh global descriptor set");
        }

        // DescriptorSet bind buffer
        VkDescriptorBufferInfo mesh_perframe_storage_buffer_info = {};
        mesh_perframe_storage_buffer_info.offset                 = 0;
        mesh_perframe_storage_buffer_info.range = sizeof(MeshPerframeStorageBufferObject); ///< shader struct
        mesh_perframe_storage_buffer_info.buffer =
            m_global_render_resource->_storage_buffer._global_upload_ringbuffer; ///< buffer
        assert(mesh_perframe_storage_buffer_info.range <
               m_global_render_resource->_storage_buffer._max_storage_buffer_range); ///< assert

        VkDescriptorBufferInfo mesh_perdrawcall_storage_buffer_info = {};
        mesh_perdrawcall_storage_buffer_info.offset                 = 0;
        mesh_perdrawcall_storage_buffer_info.range                  = sizeof(MeshPerdrawcallStorageBufferObject);
        mesh_perdrawcall_storage_buffer_info.buffer =
            m_global_render_resource->_storage_buffer._global_upload_ringbuffer;
        assert(mesh_perdrawcall_storage_buffer_info.range <
               m_global_render_resource->_storage_buffer._max_storage_buffer_range);

        VkDescriptorImageInfo brdf_texture_image_info = {};
        brdf_texture_image_info.sampler     = m_global_render_resource->_ibl_resource._brdfLUT_texture_sampler;
        brdf_texture_image_info.imageView   = m_global_render_resource->_ibl_resource._brdfLUT_texture_image_view;
        brdf_texture_image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkDescriptorImageInfo irradiance_texture_image_info = {};
        irradiance_texture_image_info.sampler = m_global_render_resource->_ibl_resource._irradiance_texture_sampler;
        irradiance_texture_image_info.imageView =
            m_global_render_resource->_ibl_resource._irradiance_texture_image_view;
        irradiance_texture_image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkDescriptorImageInfo specular_texture_image_info {};
        specular_texture_image_info.sampler     = m_global_render_resource->_ibl_resource._specular_texture_sampler;
        specular_texture_image_info.imageView   = m_global_render_resource->_ibl_resource._specular_texture_image_view;
        specular_texture_image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet mesh_descriptor_writes_info[5];

        mesh_descriptor_writes_info[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        mesh_descriptor_writes_info[0].pNext           = NULL;
        mesh_descriptor_writes_info[0].dstSet          = m_descriptor_infos[_mesh_global].descriptor_set;
        mesh_descriptor_writes_info[0].dstBinding      = 0;
        mesh_descriptor_writes_info[0].dstArrayElement = 0;
        mesh_descriptor_writes_info[0].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        mesh_descriptor_writes_info[0].descriptorCount = 1;
        mesh_descriptor_writes_info[0].pBufferInfo     = &mesh_perframe_storage_buffer_info;

        mesh_descriptor_writes_info[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        mesh_descriptor_writes_info[1].pNext           = NULL;
        mesh_descriptor_writes_info[1].dstSet          = m_descriptor_infos[_mesh_global].descriptor_set;
        mesh_descriptor_writes_info[1].dstBinding      = 1;
        mesh_descriptor_writes_info[1].dstArrayElement = 0;
        mesh_descriptor_writes_info[1].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        mesh_descriptor_writes_info[1].descriptorCount = 1;
        mesh_descriptor_writes_info[1].pBufferInfo     = &mesh_perdrawcall_storage_buffer_info;

        mesh_descriptor_writes_info[2].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        mesh_descriptor_writes_info[2].pNext           = NULL;
        mesh_descriptor_writes_info[2].dstSet          = m_descriptor_infos[_mesh_global].descriptor_set;
        mesh_descriptor_writes_info[2].dstBinding      = 2;
        mesh_descriptor_writes_info[2].dstArrayElement = 0;
        mesh_descriptor_writes_info[2].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        mesh_descriptor_writes_info[2].descriptorCount = 1;
        mesh_descriptor_writes_info[2].pImageInfo      = &brdf_texture_image_info;

        mesh_descriptor_writes_info[3]            = mesh_descriptor_writes_info[2];
        mesh_descriptor_writes_info[3].dstBinding = 3;
        mesh_descriptor_writes_info[3].pImageInfo = &irradiance_texture_image_info;

        mesh_descriptor_writes_info[4]            = mesh_descriptor_writes_info[2];
        mesh_descriptor_writes_info[4].dstBinding = 4;
        mesh_descriptor_writes_info[4].pImageInfo = &specular_texture_image_info;

        vkUpdateDescriptorSets(m_vulkan_api->getLogicDevice(),
                               sizeof(mesh_descriptor_writes_info) / sizeof(mesh_descriptor_writes_info[0]),
                               mesh_descriptor_writes_info,
                               0,
                               NULL);
    }

    void MainCameraPass::setupSkyboxDescriptorSet()
    {
        // DescriptorSet
        VkDescriptorSetAllocateInfo skybox_descriptor_set_alloc_info;
        skybox_descriptor_set_alloc_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        skybox_descriptor_set_alloc_info.pNext              = NULL;
        skybox_descriptor_set_alloc_info.descriptorPool     = m_vulkan_api->getDescriptorPool();
        skybox_descriptor_set_alloc_info.descriptorSetCount = 1;
        skybox_descriptor_set_alloc_info.pSetLayouts        = &m_descriptor_infos[_skybox].layout;

        if (VK_SUCCESS != vkAllocateDescriptorSets(m_vulkan_api->getLogicDevice(),
                                                   &skybox_descriptor_set_alloc_info,
                                                   &m_descriptor_infos[_skybox].descriptor_set))
        {
            throw std::runtime_error("allocate skybox descriptor set");
        }

        // DescriptorSet bind buffer
        VkDescriptorBufferInfo mesh_perframe_storage_buffer_info = {};
        mesh_perframe_storage_buffer_info.offset                 = 0;
        mesh_perframe_storage_buffer_info.range                  = sizeof(MeshPerframeStorageBufferObject);
        mesh_perframe_storage_buffer_info.buffer = m_global_render_resource->_storage_buffer._global_upload_ringbuffer;
        assert(mesh_perframe_storage_buffer_info.range <
               m_global_render_resource->_storage_buffer._max_storage_buffer_range);

        VkDescriptorImageInfo specular_texture_image_info = {};
        specular_texture_image_info.sampler     = m_global_render_resource->_ibl_resource._specular_texture_sampler;
        specular_texture_image_info.imageView   = m_global_render_resource->_ibl_resource._specular_texture_image_view;
        specular_texture_image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet skybox_descriptor_writes_info[2];

        skybox_descriptor_writes_info[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        skybox_descriptor_writes_info[0].pNext           = NULL;
        skybox_descriptor_writes_info[0].dstSet          = m_descriptor_infos[_skybox].descriptor_set;
        skybox_descriptor_writes_info[0].dstBinding      = 0;
        skybox_descriptor_writes_info[0].dstArrayElement = 0;
        skybox_descriptor_writes_info[0].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        skybox_descriptor_writes_info[0].descriptorCount = 1;
        skybox_descriptor_writes_info[0].pBufferInfo     = &mesh_perframe_storage_buffer_info;

        skybox_descriptor_writes_info[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        skybox_descriptor_writes_info[1].pNext           = NULL;
        skybox_descriptor_writes_info[1].dstSet          = m_descriptor_infos[_skybox].descriptor_set;
        skybox_descriptor_writes_info[1].dstBinding      = 1;
        skybox_descriptor_writes_info[1].dstArrayElement = 0;
        skybox_descriptor_writes_info[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        skybox_descriptor_writes_info[1].descriptorCount = 1;
        skybox_descriptor_writes_info[1].pImageInfo      = &specular_texture_image_info;

        vkUpdateDescriptorSets(m_vulkan_api->getLogicDevice(), 2, skybox_descriptor_writes_info, 0, NULL);
    }

    void MainCameraPass::drawMeshLighting()
    {
        // write data
        struct MeshNode
        {
            const Matrix4x4* model_matrix {nullptr};
        };

        std::map<VulkanPBRMaterial*, std::map<VulkanMesh*, std::vector<MeshNode>>> main_camera_mesh_drawcall_batch;

        for (RenderMeshNode& node : *(m_visiable_nodes.p_main_camera_visible_mesh_nodes))
        {
            auto& mesh_instanced = main_camera_mesh_drawcall_batch[node.ref_material];
            auto& mesh_nodes     = mesh_instanced[node.ref_mesh];

            MeshNode temp;
            temp.model_matrix = node.model_matrix;

            mesh_nodes.push_back(temp);
        }

        // Bind Pipeline
        vkCmdBindPipeline(m_vulkan_api->getCurrentCommandBuffer(),
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          m_render_pipelines[_render_pipeline_type_mesh_lighting].pipeline);

        // set viewport and scissor
        SwapChainDesc swap_chain_desc = m_vulkan_api->getSwapchainInfo();
        vkCmdSetViewport(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, &swap_chain_desc.viewport);
        vkCmdSetScissor(m_vulkan_api->getCurrentCommandBuffer(), 0, 1, &swap_chain_desc.scissor);

        // perframe storage buffer
        uint32_t perframe_dynamic_offset =
            roundUp(m_global_render_resource->_storage_buffer
                        ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()],
                    m_global_render_resource->_storage_buffer._min_storage_buffer_offset_alignment); ///< alignment data

        m_global_render_resource->_storage_buffer._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] =
            perframe_dynamic_offset + sizeof(MeshPerframeStorageBufferObject); ///< Next starting offset

        assert(m_global_render_resource->_storage_buffer
                   ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] <=
               (m_global_render_resource->_storage_buffer
                    ._global_upload_ringbuffers_begin[m_vulkan_api->getCurrentFrameIndex()] +
                m_global_render_resource->_storage_buffer
                    ._global_upload_ringbuffers_size[m_vulkan_api->getCurrentFrameIndex()])); ///< Assert whether
                                                                                              ///< the bias is
                                                                                              //   unreasonable

        (*reinterpret_cast<MeshPerframeStorageBufferObject*>(
            reinterpret_cast<uintptr_t>(
                m_global_render_resource->_storage_buffer._global_upload_ringbuffer_memory_pointer) +
            perframe_dynamic_offset)) = m_mesh_perframe_storage_buffer_object; ///< write data to buffer

        // render
        for (auto& pair1 : main_camera_mesh_drawcall_batch) ///< Same material
        {
            VulkanPBRMaterial& material       = (*pair1.first); ///< VulkanPBRMaterial
            auto&              mesh_instanced = pair1.second;   ///< pair

            // bind per material
            vkCmdBindDescriptorSets(m_vulkan_api->getCurrentCommandBuffer(),
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    m_render_pipelines[_render_pipeline_type_mesh_lighting].layout,
                                    1,
                                    1,
                                    &material.material_descriptor_set,
                                    0,
                                    NULL);

            for (auto& pair2 : mesh_instanced) ///< Same mesh
            {
                VulkanMesh& mesh       = (*pair2.first); ///< VulkanMesh
                auto&       mesh_nodes = pair2.second;   ///< MeshNode

                uint32_t total_instance_count = static_cast<uint32_t>(mesh_nodes.size()); ///< mesh count
                if (total_instance_count > 0)
                {
                    // bind vertex buffer
                    VkBuffer     vertex_buffers[3] = {mesh.mesh_vertex_position_buffer,
                                                      mesh.mesh_vertex_varying_enable_blending_buffer,
                                                      mesh.mesh_vertex_varying_buffer};
                    VkDeviceSize offsets[]         = {0, 0, 0};
                    vkCmdBindVertexBuffers(m_vulkan_api->getCurrentCommandBuffer(),
                                           0,
                                           (sizeof(vertex_buffers) / sizeof(vertex_buffers[0])),
                                           vertex_buffers,
                                           offsets);
                    // bind indice buffer
                    vkCmdBindIndexBuffer(
                        m_vulkan_api->getCurrentCommandBuffer(), mesh.mesh_index_buffer, 0, VK_INDEX_TYPE_UINT32);

                    // Calculate the number of draw calls required for instance rendering
                    uint32_t drawcall_max_instance_count =
                        (sizeof(MeshPerdrawcallStorageBufferObject::mesh_instances) /
                         sizeof(MeshPerdrawcallStorageBufferObject::mesh_instances[0])); ///< Maximum number of
                                                                                         ///< instances that can be
                                                                                         ///< rendered at a time
                    uint32_t drawcall_count = roundUp(total_instance_count, drawcall_max_instance_count) /
                                              drawcall_max_instance_count; ///< drawcall count

                    for (uint32_t drawcall_index = 0; drawcall_index < drawcall_count; ++drawcall_index)
                    {
                        // Calculate the number of instances to be rendered this time
                        uint32_t current_instance_count =
                            ((total_instance_count - drawcall_max_instance_count * drawcall_index) <
                             drawcall_max_instance_count) ?
                                (total_instance_count - drawcall_max_instance_count * drawcall_index) :
                                drawcall_max_instance_count;

                        // per drawcall storage buffer
                        uint32_t perdrawcall_dynamic_offset =
                            roundUp(m_global_render_resource->_storage_buffer
                                        ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()],
                                    m_global_render_resource->_storage_buffer
                                        ._min_storage_buffer_offset_alignment); ///< alignment data

                        m_global_render_resource->_storage_buffer
                            ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] =
                            perdrawcall_dynamic_offset +
                            sizeof(MeshPerdrawcallStorageBufferObject); ///< Next starting offset

                        assert(m_global_render_resource->_storage_buffer
                                   ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] <=
                               (m_global_render_resource->_storage_buffer
                                    ._global_upload_ringbuffers_begin[m_vulkan_api->getCurrentFrameIndex()] +
                                m_global_render_resource->_storage_buffer._global_upload_ringbuffers_size
                                    [m_vulkan_api->getCurrentFrameIndex()])); ///< Assert whether
                                                                              ///< the bias is
                                                                              ///<    unreasonable

                        MeshPerdrawcallStorageBufferObject& perdrawcall_storage_buffer_object =
                            (*reinterpret_cast<MeshPerdrawcallStorageBufferObject*>(
                                reinterpret_cast<uintptr_t>(m_global_render_resource->_storage_buffer
                                                                ._global_upload_ringbuffer_memory_pointer) +
                                perdrawcall_dynamic_offset));
                        for (uint32_t i = 0; i < current_instance_count; ++i)
                        {
                            perdrawcall_storage_buffer_object.mesh_instances[i].model_matrix =
                                *mesh_nodes[drawcall_max_instance_count * drawcall_index + i]
                                     .model_matrix; ///< write data to buffer
                        }

                        // Bind DescriptorSet
                        uint32_t dynamic_offsets[2] = {perframe_dynamic_offset, perdrawcall_dynamic_offset};
                        vkCmdBindDescriptorSets(m_vulkan_api->getCurrentCommandBuffer(),
                                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                m_render_pipelines[_render_pipeline_type_mesh_lighting].layout,
                                                0,
                                                1,
                                                &m_descriptor_infos[_mesh_global].descriptor_set,
                                                2,
                                                dynamic_offsets);

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
    }

    void MainCameraPass::drawSkybox()
    {
        // perframe storage buffer
        uint32_t perframe_dynamic_offset =
            roundUp(m_global_render_resource->_storage_buffer
                        ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()],
                    m_global_render_resource->_storage_buffer._min_storage_buffer_offset_alignment); ///< alignment data

        m_global_render_resource->_storage_buffer._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] =
            perframe_dynamic_offset + sizeof(MeshPerframeStorageBufferObject); ///< Next starting offset

        assert(m_global_render_resource->_storage_buffer
                   ._global_upload_ringbuffers_end[m_vulkan_api->getCurrentFrameIndex()] <=
               (m_global_render_resource->_storage_buffer
                    ._global_upload_ringbuffers_begin[m_vulkan_api->getCurrentFrameIndex()] +
                m_global_render_resource->_storage_buffer
                    ._global_upload_ringbuffers_size[m_vulkan_api->getCurrentFrameIndex()])); /// assert

        (*reinterpret_cast<MeshPerframeStorageBufferObject*>(
            reinterpret_cast<uintptr_t>(
                m_global_render_resource->_storage_buffer._global_upload_ringbuffer_memory_pointer) +
            perframe_dynamic_offset)) = m_mesh_perframe_storage_buffer_object; ///< write data to buffer

        // bind pipline
        vkCmdBindPipeline(m_vulkan_api->getCurrentCommandBuffer(),
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          m_render_pipelines[_render_pipeline_type_skybox].pipeline);

        vkCmdBindDescriptorSets(m_vulkan_api->getCurrentCommandBuffer(),
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                m_render_pipelines[_render_pipeline_type_skybox].layout,
                                0,
                                1,
                                &m_descriptor_infos[_skybox].descriptor_set,
                                1,
                                &perframe_dynamic_offset);
        vkCmdDraw(m_vulkan_api->getCurrentCommandBuffer(), 36, 1, 0, 0); // 2 triangles(6 vertex) each face, 6 faces
    }

} // namespace VKernel

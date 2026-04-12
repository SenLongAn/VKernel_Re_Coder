#include "runtime/function/render/passes/ui_pass.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/ui/window_ui.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

namespace VKernel
{
    void UIPass::initialize(const RenderPassInitInfo *init_info)
    {
        RenderPass::initialize(nullptr);

        // create imgui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Cast base class info to derived class info, and get the render pass
        m_framebuffer.render_pass = static_cast<const UIPassInitInfo *>(init_info)->render_pass;
    }

    void UIPass::initializeUIRenderBackend()
    {
        // Integration of ImGui and GLFW, This allows ImGui to respond to user input.
        ImGui_ImplGlfw_InitForVulkan(m_vulkan_api->getWindow(), true);

        // Integration of ImGui and Vulkan, allowing imgui to be rendered
        // Vulkan object sets ImGui init info
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = m_vulkan_api->getInstance();
        init_info.PhysicalDevice = m_vulkan_api->getPhysicalDevice();
        init_info.Device = m_vulkan_api->getLogicDevice();
        init_info.QueueFamily = m_vulkan_api->getQueueFamilyIndices().graphics_family.value();
        init_info.Queue = m_vulkan_api->getGraphicsQueue();
        init_info.DescriptorPool = m_vulkan_api->getDescriptorPool();
        init_info.Subpass = _main_camera_subpass_ui;
        init_info.MinImageCount = 3;
        init_info.ImageCount = 3;
        ImGui_ImplVulkan_Init(&init_info, m_framebuffer.render_pass);

        // upload font
        uploadFonts();
    }

    void UIPass::draw()
    {
        // update font
        g_runtime_global_context.m_window_ui_manager->preUpdate();
        uploadFonts();

        // If the Editor UI Settings exist
        // Order Management
        ImGui_ImplVulkan_NewFrame(); ///< Start a new Vulkan frame
        ImGui_ImplGlfw_NewFrame();   ///< Start a new GLFW frame
        ImGui::NewFrame();           ///< Start a new ImGui frame

        // render editor ui
        g_runtime_global_context.m_window_ui_manager->updateUIs();

        ImGui::Render(); ///< Convert UI descriptions into GPU-renderable data

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                        m_vulkan_api->getCurrentCommandBuffer()); ///< Submit UI data to Vulkan
    }

    void UIPass::uploadFonts()
    {
        ImGui_ImplVulkan_DestroyFontsTexture();

        // begin
        VkCommandBuffer commandBuffer = m_vulkan_api->beginSingleTimeCommands();

        // create font texture
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

        // end
        m_vulkan_api->endSingleTimeCommands(commandBuffer);

        // destory
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
} // namespace VKernel

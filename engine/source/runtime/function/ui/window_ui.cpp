#include "runtime/function/ui/window_ui.h"

#include "runtime/engine.h"
#include "runtime/function/render/render_type.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/render_resource_base.h"
#include "runtime/core/base/macro.h"
#include "runtime/core/font/IconsFontAwesome6.h"

#include <backends/imgui_impl_vulkan.h>
#include "window_ui.h"

namespace VKernel
{
    bool WindowUI::selected_object = true;

    void WindowUI::createImGuiDescriptorSetForTexture(VkImage &image,
                                                      VkImageView &image_view,
                                                      VmaAllocation &image_allocation,
                                                      VkDescriptorSetLayout &descriptorSetLayout,
                                                      VkDescriptorSet &descriptorSet,
                                                      ImTextureID &texture_id,
                                                      std::string path,
                                                      bool is_srgb)
    {
        std::shared_ptr<VulkanAPI> vulkan_api = g_runtime_global_context.m_render_system->getVulkanAPI();

        // load texture data
        std::shared_ptr<TextureData> texture = g_runtime_global_context.m_render_system->getRenderResource()->loadTexture(
            path.c_str(), is_srgb);
        if (texture && texture->m_pixels)
        {
            unsigned char *pixel_data = reinterpret_cast<unsigned char *>(texture->m_pixels);
            int pixel_count = texture->m_width * texture->m_height;

            for (int i = 0; i < pixel_count; i++)
            {
                if (pixel_data[i * 4 + 0] < 1 && pixel_data[i * 4 + 1] < 1 && pixel_data[i * 4 + 2] < 1)
                {
                    pixel_data[i * 4 + 0] = 1;
                    pixel_data[i * 4 + 1] = 1;
                    pixel_data[i * 4 + 2] = 1;
                }
                if (pixel_data[i * 4 + 0] < 1)
                {
                    pixel_data[i * 4 + 0] = 1;
                }
                if (pixel_data[i * 4 + 1] < 1)
                {
                    pixel_data[i * 4 + 1] = 1;
                }
                if (pixel_data[i * 4 + 1] < 1)
                {
                    pixel_data[i * 4 + 1] = 1;
                }
            }
        }

        // create image、imageView
        vulkan_api->createGlobalImage(image,
                                      image_view,
                                      image_allocation,
                                      texture->m_width,
                                      texture->m_height,
                                      texture->m_pixels,
                                      texture->m_format);

        // create DescriptorSetLayout
        descriptorSet = ImGui_ImplVulkan_AddTexture(g_runtime_global_context.m_render_system->getVulkanAPI()->getOrCreateDefaultSampler(Default_Sampler_Linear),
                                                    image_view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        texture_id = (ImTextureID)descriptorSet;
    }

    bool WindowUI::isMouseInWindowRange()
    {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        ImVec2 window_pos = ImGui::GetWindowPos();
        ImVec2 window_size = ImGui::GetWindowSize();

        return (mouse_pos.x >= window_pos.x &&
                mouse_pos.x <= window_pos.x + window_size.x &&
                mouse_pos.y >= window_pos.y &&
                mouse_pos.y <= window_pos.y + window_size.y);
    }

    void WindowUI::addSeparator()
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPos().y + ImGui::GetWindowSize().y / 50.0f);

        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImVec2 start = ImGui::GetCursorScreenPos();
        ImVec2 end = ImVec2(start.x + ImGui::GetWindowSize().x, start.y);
        draw_list->AddLine(start, end, IM_COL32(0, 200, 200, 255), ImGui::GetWindowSize().y / 700.0f);

        ImGui::SetCursorPosY(ImGui::GetCursorPos().y + ImGui::GetWindowSize().y / 100.0f);
    }

    void WindowUI::updateFont(const std::string &path, float size)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.Fonts->Clear();

        io.Fonts->AddFontFromFileTTF(path.c_str(), size);

        float baseFontSize = 20.0f;
        float iconFontSize = baseFontSize * 2.0f / 3.0f;
        static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = iconFontSize;
        std::string fontPath = "asset/font/" + std::string(FONT_ICON_FILE_NAME_FAS);
        io.Fonts->AddFontFromFileTTF(fontPath.c_str(), iconFontSize, &icons_config, icons_ranges);

        io.Fonts->Build();
    }

    int WindowUIFactory::index = 0;

    void WindowUIManager::registerUIs() { m_uis = WindowUIFactory::getInstance().createUIs(); }

    void WindowUIManager::initUIs(WindowUIInitInfo init_info)
    {
        for (auto &ui : m_uis)
        {
            ui->initialize(init_info);
        }
    }

    void WindowUIManager::preUpdate()
    {
        for (auto &ui : m_uis)
        {
            if (VKernel::g_is_editor_mode && ui->iSGameMode())
                continue;
            if (!VKernel::g_is_editor_mode && !ui->iSGameMode())
                continue;
            ui->preUpdate();
        }
    }

    void WindowUIManager::updateUIs()
    {
        WindowUI::selected_object = true;

        for (auto &ui : m_uis)
        {
            if (VKernel::g_is_editor_mode && ui->iSGameMode())
                continue;
            ui->preRender();
        }
    }

    WindowUIManager::~WindowUIManager() { m_uis.clear(); }
} // namespace VKernel

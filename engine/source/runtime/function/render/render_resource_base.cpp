#include "runtime/function/render/render_resource_base.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace VKernel
{
    std::shared_ptr<TextureData> RenderResourceBase::loadTexture(std::string file, bool is_srgb)
    {
        std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();

        int iw, ih, n;
        texture->m_pixels = stbi_load(file.c_str(), &iw, &ih, &n, 4);

        if (!texture->m_pixels)
            return nullptr;

        texture->m_width = iw;
        texture->m_height = ih;
        texture->m_format = (is_srgb) ? VkFormat::VK_FORMAT_R8G8B8A8_SRGB : VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
        texture->m_depth = 1;
        texture->m_array_layers = 1;
        texture->m_mip_levels = 1;
        texture->m_type = IMAGE_TYPE::IMAGE_TYPE_2D;

        return texture;
    }

}

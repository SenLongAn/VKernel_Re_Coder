#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <vulkan/vulkan.h>

namespace VKernel
{
    enum class IMAGE_TYPE : uint8_t
    {
        IMAGE_TYPE_UNKNOWM = 0,
        IMAGE_TYPE_2D
    };

    enum DefaultSamplerType ///< image sampler type
    {
        Default_Sampler_Linear,
        Default_Sampler_Nearest
    };

    class TextureData ///< texture data
    {
    public:
        uint32_t m_width{0};
        uint32_t m_height{0};
        uint32_t m_depth{0};
        uint32_t m_mip_levels{0};
        uint32_t m_array_layers{0};
        void *m_pixels{nullptr}; ///< data

        VkFormat m_format = VK_FORMAT_MAX_ENUM;
        IMAGE_TYPE m_type{IMAGE_TYPE::IMAGE_TYPE_UNKNOWM};

        TextureData() = default;
        ~TextureData()
        {
            if (m_pixels)
            {
                free(m_pixels);
            }
        }
        bool isValid() const { return m_pixels != nullptr; }
    };
}
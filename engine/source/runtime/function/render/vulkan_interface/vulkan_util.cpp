#include "runtime/function/render/vulkan_interface/vulkan_util.h"

#include <iostream>

namespace VKernel
{
    uint32_t VulkanUtil::findMemoryType(VkPhysicalDevice      physical_device,
                                        uint32_t              type_filter,
                                        VkMemoryPropertyFlags properties_flag)
    {
        // Iterative physical properties
        VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_device, &physical_device_memory_properties);
        for (uint32_t i = 0; i < physical_device_memory_properties.memoryTypeCount; i++)
        {
            // Check if there are flags for "filter" and "properties"
            if (type_filter & (1 << i) &&
                (physical_device_memory_properties.memoryTypes[i].propertyFlags & properties_flag) == properties_flag)
            {
                return i;
            }
        }
        std::cerr << "findMemoryType error" << std::endl;
        return 0;
    }

    VkShaderModule VulkanUtil::createShaderModule(VkDevice device, const std::vector<unsigned char>& shader_code)
    {
        // create Shader Module
        VkShaderModuleCreateInfo shader_module_create_info {};
        shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_create_info.codeSize = shader_code.size();
        shader_module_create_info.pCode    = reinterpret_cast<const uint32_t*>(shader_code.data());

        VkShaderModule shader_module;
        if (vkCreateShaderModule(device, &shader_module_create_info, nullptr, &shader_module) != VK_SUCCESS)
        {
            return VK_NULL_HANDLE;
        }
        return shader_module;
    }

    void VulkanUtil::createImage(VkPhysicalDevice      physical_device,
                                 VkDevice              device,
                                 uint32_t              image_width,
                                 uint32_t              image_height,
                                 VkFormat              format,
                                 VkImageTiling         image_tiling,
                                 VkImageUsageFlags     image_usage_flags,
                                 VkMemoryPropertyFlags memory_property_flags,
                                 VkImage&              image,
                                 VkDeviceMemory&       memory,
                                 VkImageCreateFlags    image_create_flags,
                                 uint32_t              array_layers,
                                 uint32_t              miplevels)
    {
        // create image
        VkImageCreateInfo image_create_info {};
        image_create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.flags         = image_create_flags;
        image_create_info.imageType     = VK_IMAGE_TYPE_2D;
        image_create_info.extent.width  = image_width;
        image_create_info.extent.height = image_height;
        image_create_info.extent.depth  = 1;
        image_create_info.mipLevels     = miplevels;
        image_create_info.arrayLayers   = array_layers;
        image_create_info.format        = format;
        image_create_info.tiling        = image_tiling;
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info.usage         = image_usage_flags;
        image_create_info.samples       = VK_SAMPLE_COUNT_1_BIT;
        image_create_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &image_create_info, nullptr, &image) != VK_SUCCESS)
        {
            std::cerr << "failed to create image!" << std::endl;
            return;
        }

        // allocate memory
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements); ///< get Requirement size

        VkMemoryAllocateInfo allocInfo {};
        allocInfo.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex =
            findMemoryType(physical_device, memRequirements.memoryTypeBits, memory_property_flags); ///< Find the indexes corresponding to the "filter" and "properties" of the requirements

        if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        {
            std::cerr << "failed to allocate image memory!" << std::endl;
            return;
        }

        vkBindImageMemory(device, image, memory, 0); ///< memory is related to image
    }

    VkImageView VulkanUtil::createImageView(VkDevice           device,
                                            VkImage&           image,
                                            VkFormat           format,
                                            VkImageAspectFlags image_aspect_flags,
                                            VkImageViewType    view_type,
                                            uint32_t           layout_count,
                                            uint32_t           miplevels)
    {
        VkImageViewCreateInfo image_view_create_info {};
        image_view_create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image                           = image;
        image_view_create_info.viewType                        = view_type;
        image_view_create_info.format                          = format;
        image_view_create_info.subresourceRange.aspectMask     = image_aspect_flags;
        image_view_create_info.subresourceRange.baseMipLevel   = 0;
        image_view_create_info.subresourceRange.levelCount     = miplevels;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount     = layout_count;

        VkImageView image_view;
        if (vkCreateImageView(device, &image_view_create_info, nullptr, &image_view) != VK_SUCCESS)
        {
            return image_view;
        }

        return image_view;
    }
}
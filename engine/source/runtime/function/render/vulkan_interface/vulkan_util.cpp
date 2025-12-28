#include "runtime/function/render/vulkan_interface/vulkan_util.h"

#include "vulkan_util.h"
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
        throw std::runtime_error("findMemoryType error");
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

    void VulkanUtil::createBuffer(VkPhysicalDevice      physical_device,
                                  VkDevice              device,
                                  VkDeviceSize          size,
                                  VkBufferUsageFlags    usage,
                                  VkMemoryPropertyFlags properties,
                                  VkBuffer&             buffer,
                                  VkDeviceMemory&       buffer_memory)
    {
        // create buffer
        VkBufferCreateInfo buffer_create_info {};
        buffer_create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_create_info.size        = size;
        buffer_create_info.usage       = usage;                     // use as a vertex/staging/index buffer
        buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // not sharing among queue families

        if (vkCreateBuffer(device, &buffer_create_info, nullptr, &buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("vkCreateBuffer failed!");
            return;
        }

        // allocate memory
        VkMemoryRequirements buffer_memory_requirements; // for allocate_info.allocationSize and
                                                         // allocate_info.memoryTypeIndex
        vkGetBufferMemoryRequirements(device, buffer, &buffer_memory_requirements);

        VkMemoryAllocateInfo buffer_memory_allocate_info {};
        buffer_memory_allocate_info.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        buffer_memory_allocate_info.allocationSize = buffer_memory_requirements.size;
        buffer_memory_allocate_info.memoryTypeIndex =
            VulkanUtil::findMemoryType(physical_device, buffer_memory_requirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &buffer_memory_allocate_info, nullptr, &buffer_memory) != VK_SUCCESS)
        {
            throw std::runtime_error("vkAllocateMemory failed!");
            return;
        }

        // bind buffer with buffer memory
        vkBindBufferMemory(device, buffer, buffer_memory, 0); // offset = 0
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
        VkImageCreateInfo vulkan_api {};
        vulkan_api.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        vulkan_api.flags         = image_create_flags;
        vulkan_api.imageType     = VK_IMAGE_TYPE_2D;
        vulkan_api.extent.width  = image_width;
        vulkan_api.extent.height = image_height;
        vulkan_api.extent.depth  = 1;
        vulkan_api.mipLevels     = miplevels;
        vulkan_api.arrayLayers   = array_layers;
        vulkan_api.format        = format;
        vulkan_api.tiling        = image_tiling;
        vulkan_api.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        vulkan_api.usage         = image_usage_flags;
        vulkan_api.samples       = VK_SAMPLE_COUNT_1_BIT;
        vulkan_api.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &vulkan_api, nullptr, &image) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image!");
            return;
        }

        // allocate memory
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements); ///< get Requirement size

        VkMemoryAllocateInfo allocInfo {};
        allocInfo.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex =
            findMemoryType(physical_device,
                           memRequirements.memoryTypeBits,
                           memory_property_flags); ///< Find the indexes corresponding to the "filter" and "properties"
                                                   ///< of the requirements

        if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate image memory!");
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
        image_view_create_info.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
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

    VkSampler
    VulkanUtil::createMipmapSampler(VkPhysicalDevice physical_device, VkDevice device, uint32_t width, uint32_t height)
    {
        VkSampler sampler;

        uint32_t mip_levels = floor(log2(std::max(width, height))) + 1;

        VkPhysicalDeviceProperties physical_device_properties {};
        vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);

        VkSamplerCreateInfo samplerInfo {};

        samplerInfo.sType        = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter    = VK_FILTER_LINEAR;
        samplerInfo.minFilter    = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy    = physical_device_properties.limits.maxSamplerAnisotropy; // close :1.0f

        samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable           = VK_FALSE;
        samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        samplerInfo.maxLod = mip_levels - 1;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
        {
            throw std::runtime_error("vk create sampler");
        }

        return sampler;
    }

    VkSampler VulkanUtil::createSampler(VkPhysicalDevice physical_device, VkDevice device, const VkFilter& filter)
    {
        VkSampler sampler;

        VkPhysicalDeviceProperties physical_device_properties {};
        vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);

        VkSamplerCreateInfo samplerInfo {};

        samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter               = filter;
        samplerInfo.minFilter               = filter;
        samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        samplerInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.mipLodBias              = 0.0f;
        samplerInfo.anisotropyEnable        = VK_FALSE;
        samplerInfo.maxAnisotropy           = physical_device_properties.limits.maxSamplerAnisotropy; // close :1.0f
        samplerInfo.compareEnable           = VK_FALSE;
        samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
        samplerInfo.minLod                  = 0.0f;
        samplerInfo.maxLod                  = 8.0f; // todo: m_irradiance_texture_miplevels
        samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
        {
            throw std::runtime_error("vk create sampler");
        }

        return sampler;
    }

    void VulkanUtil::createGlobalImage(VulkanAPI*     vulkan_api,
                                       VkImage&       image,
                                       VkImageView&   image_view,
                                       VmaAllocation& image_allocation,
                                       uint32_t       texture_image_width,
                                       uint32_t       texture_image_height,
                                       void*          texture_image_pixels,
                                       VkFormat       texture_image_format,
                                       uint32_t       miplevels)
    {
        // Return if there is no data
        if (!texture_image_pixels)
        {
            return;
        }

        // Calculate texture size based on image format
        VkDeviceSize texture_byte_size;
        switch (texture_image_format)
        {
            case VkFormat::VK_FORMAT_R8G8B8_UNORM:
                texture_byte_size = texture_image_width * texture_image_height * 3;
                break;
            case VkFormat::VK_FORMAT_R8G8B8_SRGB:
                texture_byte_size = texture_image_width * texture_image_height * 3;
                break;
            case VkFormat::VK_FORMAT_R8G8B8A8_UNORM:
                texture_byte_size = texture_image_width * texture_image_height * 4;
                break;
            case VkFormat::VK_FORMAT_R8G8B8A8_SRGB:
                texture_byte_size = texture_image_width * texture_image_height * 4;
                break;
            case VkFormat::VK_FORMAT_R32_SFLOAT:
                texture_byte_size = texture_image_width * texture_image_height * 4;
                break;
            case VkFormat::VK_FORMAT_R32G32_SFLOAT:
                texture_byte_size = texture_image_width * texture_image_height * 4 * 2;
                break;
            case VkFormat::VK_FORMAT_R32G32B32_SFLOAT:
                texture_byte_size = texture_image_width * texture_image_height * 4 * 3;
                break;
            case VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT:
                texture_byte_size = texture_image_width * texture_image_height * 4 * 4;
                break;
            default:
                throw std::runtime_error("invalid texture_byte_size");
                break;
        }

        // staging buffer
        VkBuffer       inefficient_staging_buffer;
        VkDeviceMemory inefficient_staging_buffer_memory;
        VulkanUtil::createBuffer(vulkan_api->getPhysicalDevice(),
                                 vulkan_api->getLogicDevice(),
                                 texture_byte_size,
                                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                 inefficient_staging_buffer,
                                 inefficient_staging_buffer_memory); ///< create

        void* data; ///< Map data to memory
        vkMapMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory, 0, texture_byte_size, 0, &data);
        memcpy(data, texture_image_pixels, static_cast<size_t>(texture_byte_size));
        vkUnmapMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory);

        // Calculate mip level，Automatically calculate the maximum generation quantity if the parameter is 0
        uint32_t mip_levels =
            (miplevels != 0) ? miplevels : floor(log2(std::max(texture_image_width, texture_image_height))) + 1;

        // use the vmaAllocator to allocate asset texture image
        VkImageCreateInfo image_create_info {};
        image_create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.flags         = 0;
        image_create_info.imageType     = VK_IMAGE_TYPE_2D;
        image_create_info.extent.width  = texture_image_width;
        image_create_info.extent.height = texture_image_height;
        image_create_info.extent.depth  = 1;
        image_create_info.mipLevels     = mip_levels;
        image_create_info.arrayLayers   = 1;
        image_create_info.format        = texture_image_format;
        image_create_info.tiling        = VK_IMAGE_TILING_OPTIMAL;
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info.usage =
            VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        image_create_info.samples     = VK_SAMPLE_COUNT_1_BIT;
        image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage                   = VMA_MEMORY_USAGE_GPU_ONLY;

        vmaCreateImage(vulkan_api->getVmaAllocator(), &image_create_info, &allocInfo, &image, &image_allocation, NULL);

        // transitions image layout，As a transmission target
        transitionImageLayout(vulkan_api,
                              image,
                              VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              1,
                              1,
                              VK_IMAGE_ASPECT_COLOR_BIT);

        // Copy from staging buffer to image
        copyBufferToImage(vulkan_api, inefficient_staging_buffer, image, texture_image_width, texture_image_height, 1);

        // transitions image layout, Read by shader
        transitionImageLayout(vulkan_api,
                              image,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                              1,
                              1,
                              VK_IMAGE_ASPECT_COLOR_BIT);

        // destory staging buffer
        vkDestroyBuffer(vulkan_api->getLogicDevice(), inefficient_staging_buffer, nullptr);
        vkFreeMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory, nullptr);

        // generate mipmapped image
        genMipmappedImage(vulkan_api, image, texture_image_width, texture_image_height, mip_levels);

        // create image view
        image_view = createImageView(vulkan_api->getLogicDevice(),
                                     image,
                                     texture_image_format,
                                     VK_IMAGE_ASPECT_COLOR_BIT,
                                     VK_IMAGE_VIEW_TYPE_2D,
                                     1,
                                     mip_levels);
    }

    void VulkanUtil::genMipmappedImage(VulkanAPI* vulkan_api,
                                       VkImage    image,
                                       uint32_t   width,
                                       uint32_t   height,
                                       uint32_t   mip_levels)
    {
        if (vulkan_api == nullptr)
        {
            throw std::runtime_error("vulkan_api is nullptr");
            return;
        }

        // begin command buffer
        VkCommandBuffer command_buffer = vulkan_api->beginSingleTimeCommands();

        // Generate
        for (uint32_t i = 1; i < mip_levels; i++) ///< Loop mipLevels - 1 times
        {
            // Specify the Blit range and mipmap level
            VkImageBlit imageBlit {};
            imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBlit.srcSubresource.layerCount = 1;
            imageBlit.srcSubresource.mipLevel   = i - 1;
            imageBlit.srcOffsets[1].x           = std::max((int32_t)(width >> (i - 1)), 1); ///< Division
            imageBlit.srcOffsets[1].y           = std::max((int32_t)(height >> (i - 1)), 1);
            imageBlit.srcOffsets[1].z           = 1;

            imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBlit.dstSubresource.layerCount = 1;
            imageBlit.dstSubresource.mipLevel   = i;
            imageBlit.dstOffsets[1].x           = std::max((int32_t)(width >> i), 1);
            imageBlit.dstOffsets[1].y           = std::max((int32_t)(height >> i), 1);
            imageBlit.dstOffsets[1].z           = 1;

            // transition Image Layout
            VkImageSubresourceRange mipSubRange {}; ///< Set only for a specific mipmap level
            mipSubRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;
            mipSubRange.baseMipLevel = i;
            mipSubRange.levelCount   = 1;
            mipSubRange.layerCount   = 1;

            VkImageMemoryBarrier barrier {}; ///< From undefined to transfer dst
            barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.srcAccessMask       = 0;
            barrier.dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image               = image;
            barrier.subresourceRange    = mipSubRange;

            vkCmdPipelineBarrier(command_buffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &barrier);

            // Blit
            vkCmdBlitImage(command_buffer,
                           image,
                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                           image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1,
                           &imageBlit,
                           VK_FILTER_LINEAR);

            // transition Image Layout
            barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL; ///< From transfer dst to transfer src
            barrier.newLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(command_buffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &barrier);
        }

        // transition Image Layout
        VkImageSubresourceRange mipSubRange {};
        mipSubRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;
        mipSubRange.baseMipLevel = 0;
        mipSubRange.levelCount   = mip_levels; ///< All levels
        mipSubRange.layerCount   = 1;

        VkImageMemoryBarrier barrier {}; ///< From transfer src to shader read
        barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout           = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask       = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask       = VK_ACCESS_SHADER_READ_BIT;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image               = image;
        barrier.subresourceRange    = mipSubRange;

        vkCmdPipelineBarrier(command_buffer,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &barrier);

        // end command buffer
        vulkan_api->endSingleTimeCommands(command_buffer);
    }

    void VulkanUtil::generateTextureMipMaps(VulkanAPI* vulkan_api,
                                            VkImage    image,
                                            VkFormat   image_format,
                                            uint32_t   texture_width,
                                            uint32_t   texture_height,
                                            uint32_t   layers,
                                            uint32_t   miplevels)
    {
        // Does it support linear blitting?
        VkFormatProperties format_properties;
        vkGetPhysicalDeviceFormatProperties(vulkan_api->getPhysicalDevice(), image_format, &format_properties);
        if (!(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        {
            throw std::runtime_error("generateTextureMipMaps() : linear bliting not supported!");
            return;
        }

        // begin command buffer
        VkCommandBuffer command_buffer = vulkan_api->beginSingleTimeCommands();

        VkImageMemoryBarrier barrier {};
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image                           = image;
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = layers;
        barrier.subresourceRange.levelCount     = 1;

        int32_t mipwidth  = texture_width;
        int32_t mipheight = texture_height;

        for (uint32_t i = 1; i < miplevels; i++) ///< Loop mipLevels - 1 times
        {
            // transition Image Layout (transfer dst -> transfer src)
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout                     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(command_buffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &barrier);

            // Specify the Blit range and mipmap level
            VkImageBlit blit {};
            blit.srcOffsets[0]                 = {0, 0, 0};
            blit.srcOffsets[1]                 = {mipwidth, mipheight, 1};
            blit.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel       = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount     = layers; ///< Mipmaps need to be generated for all layers

            blit.dstOffsets[0]             = {0, 0, 0};
            blit.dstOffsets[1]             = {mipwidth > 1 ? mipwidth / 2 : 1, mipheight > 1 ? mipheight / 2 : 1, 1};
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel   = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount     = layers;

            // Blit
            vkCmdBlitImage(command_buffer,
                           image,
                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                           image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1,
                           &blit,
                           VK_FILTER_LINEAR);

            // transition Image Layout (transfer src -> shader read)
            barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            vkCmdPipelineBarrier(command_buffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &barrier); // for completed miplevel, change to shader_read

            // Calculate new width and height
            if (mipwidth > 1)
                mipwidth /= 2;
            if (mipheight > 1)
                mipheight /= 2;
        }

        // transition Image Layout (miplevels - 1: transfer src -> shader read)
        barrier.subresourceRange.baseMipLevel = miplevels - 1;
        barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout                     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask                 = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(command_buffer,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &barrier);

        // end command buffer
        vulkan_api->endSingleTimeCommands(command_buffer);
    }

    void VulkanUtil::createCubeMap(VulkanAPI*           vulkan_api,
                                   VkImage&             image,
                                   VkImageView&         image_view,
                                   VmaAllocation&       image_allocation,
                                   uint32_t             texture_image_width,
                                   uint32_t             texture_image_height,
                                   std::array<void*, 6> texture_image_pixels,
                                   VkFormat             texture_image_format,
                                   uint32_t             miplevels)
    {
        // Calculate texture and cube byte size
        VkDeviceSize texture_layer_byte_size;
        VkDeviceSize cube_byte_size;
        switch (texture_image_format)
        {
            case VkFormat::VK_FORMAT_R8G8B8_UNORM:
                texture_layer_byte_size = texture_image_width * texture_image_height * 3;
                break;
            case VkFormat::VK_FORMAT_R8G8B8_SRGB:
                texture_layer_byte_size = texture_image_width * texture_image_height * 3;
                break;
            case VkFormat::VK_FORMAT_R8G8B8A8_UNORM:
                texture_layer_byte_size = texture_image_width * texture_image_height * 4;
                break;
            case VkFormat::VK_FORMAT_R8G8B8A8_SRGB:
                texture_layer_byte_size = texture_image_width * texture_image_height * 4;
                break;
            case VkFormat::VK_FORMAT_R32G32_SFLOAT:
                texture_layer_byte_size = texture_image_width * texture_image_height * 4 * 2;
                break;
            case VkFormat::VK_FORMAT_R32G32B32_SFLOAT:
                texture_layer_byte_size = texture_image_width * texture_image_height * 4 * 3;
                break;
            case VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT:
                texture_layer_byte_size = texture_image_width * texture_image_height * 4 * 4;
                break;
            default:
                texture_layer_byte_size = VkDeviceSize(-1);
                throw std::runtime_error("invalid texture_layer_byte_size");
                return;
                break;
        }
        cube_byte_size = texture_layer_byte_size * 6;

        // create cubemap texture image
        VkImageCreateInfo image_create_info {};
        image_create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.flags         = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        image_create_info.imageType     = VK_IMAGE_TYPE_2D;
        image_create_info.extent.width  = static_cast<uint32_t>(texture_image_width);
        image_create_info.extent.height = static_cast<uint32_t>(texture_image_height);
        image_create_info.extent.depth  = 1;
        image_create_info.mipLevels     = miplevels;
        image_create_info.arrayLayers   = 6;
        image_create_info.format        = texture_image_format;
        image_create_info.tiling        = VK_IMAGE_TILING_OPTIMAL;
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info.usage =
            VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        image_create_info.samples     = VK_SAMPLE_COUNT_1_BIT;
        image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage                   = VMA_MEMORY_USAGE_GPU_ONLY;

        vmaCreateImage(vulkan_api->getVmaAllocator(), &image_create_info, &allocInfo, &image, &image_allocation, NULL);

        // create staging buffer
        VkBuffer       inefficient_staging_buffer;
        VkDeviceMemory inefficient_staging_buffer_memory;
        createBuffer(vulkan_api->getPhysicalDevice(),
                     vulkan_api->getLogicDevice(),
                     cube_byte_size,
                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     inefficient_staging_buffer,
                     inefficient_staging_buffer_memory);

        // write data to staging buffer
        void* data = NULL;
        vkMapMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory, 0, cube_byte_size, 0, &data);
        for (int i = 0; i < 6; i++)
        {
            memcpy((void*)(static_cast<char*>(data) + texture_layer_byte_size * i),
                   texture_image_pixels[i],
                   static_cast<size_t>(texture_layer_byte_size));
        }
        vkUnmapMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory);

        // image layout transitions -- image layout is set from none to destination
        transitionImageLayout(vulkan_api,
                              image,
                              VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              6,
                              miplevels,
                              VK_IMAGE_ASPECT_COLOR_BIT);

        // copy from staging buffer to image
        copyBufferToImage(vulkan_api,
                          inefficient_staging_buffer,
                          image,
                          static_cast<uint32_t>(texture_image_width),
                          static_cast<uint32_t>(texture_image_height),
                          6);

        // destory staging
        vkDestroyBuffer(vulkan_api->getLogicDevice(), inefficient_staging_buffer, nullptr);
        vkFreeMemory(vulkan_api->getLogicDevice(), inefficient_staging_buffer_memory, nullptr);

        // generate mipmap
        generateTextureMipMaps(
            vulkan_api, image, texture_image_format, texture_image_width, texture_image_height, 6, miplevels);

        // create image view
        image_view = createImageView(vulkan_api->getLogicDevice(),
                                     image,
                                     texture_image_format,
                                     VK_IMAGE_ASPECT_COLOR_BIT,
                                     VK_IMAGE_VIEW_TYPE_CUBE,
                                     6,
                                     miplevels);
    }

    void VulkanUtil::transitionImageLayout(VulkanAPI*         vulkan_api,
                                           VkImage            image,
                                           VkImageLayout      old_layout,
                                           VkImageLayout      new_layout,
                                           uint32_t           layer_count,
                                           uint32_t           miplevels,
                                           VkImageAspectFlags aspect_mask_bits)
    {
        if (vulkan_api == nullptr)
        {
            throw std::runtime_error("vulkan_api is nullptr");
            return;
        }

        // begin command buffer
        VkCommandBuffer command_buffer = vulkan_api->beginSingleTimeCommands();

        // barrier
        VkImageMemoryBarrier barrier {};
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout                       = old_layout;
        barrier.newLayout                       = new_layout;
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                           = image;
        barrier.subresourceRange.aspectMask     = aspect_mask_bits;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = miplevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = layer_count;

        // Set up barrier synchronization according to the old and new layouts
        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                 new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (old_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL &&
                 new_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            sourceStage      = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL &&
                 new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                 new_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else
        {
            throw std::runtime_error("unsupported layout transition!");
            return;
        }

        // transition
        vkCmdPipelineBarrier(command_buffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        // end command buffer
        vulkan_api->endSingleTimeCommands(command_buffer);
    }

    void VulkanUtil::copyBuffer(VulkanAPI*   vulkan_api,
                                VkBuffer     srcBuffer,
                                VkBuffer     dstBuffer,
                                VkDeviceSize srcOffset,
                                VkDeviceSize dstOffset,
                                VkDeviceSize size)
    {
        if (vulkan_api == nullptr)
        {
            throw std::runtime_error("vulkan_api is nullptr");
            return;
        }

        VkCommandBuffer command_buffer = vulkan_api->beginSingleTimeCommands();

        VkBufferCopy copyRegion = {srcOffset, dstOffset, size};
        vkCmdCopyBuffer(command_buffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vulkan_api->endSingleTimeCommands(command_buffer);
    }

    void VulkanUtil::copyBufferToImage(VulkanAPI* vulkan_api,
                                       VkBuffer   buffer,
                                       VkImage    image,
                                       uint32_t   width,
                                       uint32_t   height,
                                       uint32_t   layer_count)
    {
        if (vulkan_api == nullptr)
        {
            throw std::runtime_error("vulkan_api is nullptr");
            return;
        }

        // begin command buffer
        VkCommandBuffer command_buffer = vulkan_api->beginSingleTimeCommands();

        // Copy from buffer to image
        VkBufferImageCopy region {};
        region.bufferOffset                    = 0;
        region.bufferRowLength                 = 0;
        region.bufferImageHeight               = 0;
        region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel       = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount     = layer_count;
        region.imageOffset                     = {0, 0, 0};
        region.imageExtent                     = {width, height, 1};

        vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        // end command buffer
        vulkan_api->endSingleTimeCommands(command_buffer);
    }
} // namespace VKernel

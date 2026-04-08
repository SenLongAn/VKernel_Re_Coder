#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <array>
#include <vector>

/**
 * image and buffer
 */
namespace VKernel
{
    class VulkanUtil
    {

    public:
        // find
        static uint32_t
        findMemoryType(VkPhysicalDevice physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties_flag);

        // create
        static VkShaderModule createShaderModule(VkDevice device, const std::vector<unsigned char> &shader_code);

        static void createBuffer(VkPhysicalDevice physical_device,
                                 VkDevice device,
                                 VkDeviceSize size,
                                 VkBufferUsageFlags usage,
                                 VkMemoryPropertyFlags properties,
                                 VkBuffer &buffer,
                                 VkDeviceMemory &buffer_memor);

        static void createImage(VkPhysicalDevice physical_device,
                                VkDevice device,
                                uint32_t image_width,
                                uint32_t image_height,
                                VkFormat format,
                                VkImageTiling image_tiling,
                                VkImageUsageFlags image_usage_flags,
                                VkMemoryPropertyFlags memory_property_flags,
                                VkImage &image,
                                VkDeviceMemory &memory,
                                VkImageCreateFlags image_create_flags,
                                uint32_t array_layers,
                                uint32_t miplevels);

        static VkImageView createImageView(VkDevice device,
                                           VkImage &image,
                                           VkFormat format,
                                           VkImageAspectFlags image_aspect_flags,
                                           VkImageViewType view_type,
                                           uint32_t layout_count,
                                           uint32_t miplevels);

        static VkSampler
        createMipmapSampler(VkPhysicalDevice physical_device, VkDevice device, uint32_t width, uint32_t height);

        static VkSampler createSampler(VkPhysicalDevice physical_device,
                                       VkDevice device,
                                       const VkFilter &filter); ///< Create a sampler based on the filter

        static void
        createGlobalImage(VulkanAPI *vulkan_api,
                          VkImage &image,
                          VkImageView &image_view,
                          VmaAllocation &image_allocation,
                          uint32_t texture_image_width,
                          uint32_t texture_image_height,
                          void *texture_image_pixels,
                          VkFormat texture_image_format,
                          uint32_t miplevels = 0); ///< Create a image, You can specify the number of mipmaps, and if
                                                   ///< set to 0, it will be calculated automatically.

        static void genMipmappedImage(VulkanAPI *vulkan_api,
                                      VkImage image,
                                      uint32_t width,
                                      uint32_t height,
                                      uint32_t mip_levels); ///< Mipmaps of the image

        static void generateTextureMipMaps(VulkanAPI *vulkan_api,
                                           VkImage image,
                                           VkFormat image_format,
                                           uint32_t texture_width,
                                           uint32_t texture_height,
                                           uint32_t layers,
                                           uint32_t miplevels); ///< Mipmaps of the cubemap

        static void createCubeMap(VulkanAPI *vulkan_api,
                                  VkImage &image,
                                  VkImageView &image_view,
                                  VmaAllocation &image_allocation,
                                  uint32_t texture_image_width,
                                  uint32_t texture_image_height,
                                  std::array<void *, 6> texture_image_pixels,
                                  VkFormat texture_image_format,
                                  uint32_t miplevels);

        // transition
        static void transitionImageLayout(VulkanAPI *vulkan_api,
                                          VkImage image,
                                          VkImageLayout old_layout,
                                          VkImageLayout new_layout,
                                          uint32_t layer_count,
                                          uint32_t miplevels,
                                          VkImageAspectFlags aspect_mask_bits); ///< transition Image Layout
        // copy
        static void copyBuffer(VulkanAPI *vulkan_api,
                               VkBuffer srcBuffer,
                               VkBuffer dstBuffer,
                               VkDeviceSize srcOffset,
                               VkDeviceSize dstOffset,
                               VkDeviceSize size);

        static void copyBufferToImage(VulkanAPI *vulkan_api,
                                      VkBuffer buffer,
                                      VkImage image,
                                      uint32_t width,
                                      uint32_t height,
                                      uint32_t layer_count);
    };
} // namespace VKernel
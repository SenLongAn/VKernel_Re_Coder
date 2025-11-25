#pragma once

#include <optional>
#include <vector>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

/**
 * Vulkan Custom Structs
 */
namespace VKernel
{
    
    struct QueueFamilyIndices ///< Queue Family Indices
    {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool isComplete() { return graphics_family.has_value() && present_family.has_value() ; }
    };

    struct SwapChainSupportDetails ///< SwapChain Support Details
    {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    struct SwapChainDesc ///< swapchain description
    {
        VkExtent2D extent;
        VkFormat   image_format;
        VkViewport viewport;
        VkRect2D scissor;
        std::vector<VkImageView> imageViews;
    };

    struct DepthImageDesc ///< depth iamge description
    {
        VkImage depth_image = VK_NULL_HANDLE;
        VkImageView depth_image_view = VK_NULL_HANDLE;
        VkFormat        depth_image_format;
    };
}
   
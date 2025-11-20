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
        std::optional<uint32_t> m_compute_family;

        bool isComplete() { return graphics_family.has_value() && present_family.has_value() && m_compute_family.has_value();; }
    };

    struct SwapChainSupportDetails ///< SwapChain Support Details
    {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };
}
   
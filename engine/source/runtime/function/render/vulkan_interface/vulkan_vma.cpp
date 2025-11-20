/**
 * VMA: Simplify Memory Allocation
 */

#define VMA_IMPLEMENTATION 1 ///< VAM needs to include implementation.
#define VMA_STATIC_VULKAN_FUNCTIONS 0 ///< VMA dynamically obtains the function pointers through vkGetInstanceProcAddr/vkGetDeviceProcAddr.
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1 ///< No need to manually provide the function pointer
#include "vk_mem_alloc.h" ///< VAM 
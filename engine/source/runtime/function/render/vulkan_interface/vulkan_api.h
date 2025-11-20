#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_struct.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <vector>
#include <memory>

/**
 * Vulkan interface
 */
namespace VKernel
{

    class WindowSystem;
    
    class VulkanAPI final ///< vulkan API
    {
        
    public:
        void initialize(std::shared_ptr<WindowSystem> window_system); ///< init
        ~VulkanAPI(); ///< Destructor
        
    public:
        // destory
        void clear();
        
    private:
        // Maximum parallel frame count
        static uint8_t const k_max_frames_in_flight {3};

    private:
        // external reference
        GLFWwindow* m_window {nullptr};
    
        // Vulkan object
        VkInstance m_instance {nullptr};
        VkSurfaceKHR m_surface {nullptr};
        VkPhysicalDevice m_physical_device {nullptr};
        VkDevice m_device {nullptr};
        VkQueue m_graphics_queue{ nullptr };
        VkQueue m_present_queue {nullptr};
        VkQueue m_compute_queue{ nullptr };
        VkCommandPool m_command_pools[k_max_frames_in_flight];
        VkCommandBuffer m_command_buffers[k_max_frames_in_flight];
        VkDescriptorPool m_descriptor_pool{nullptr};
        
    private:
        // auxiliary configuration
        // vulkan version
        uint32_t m_vulkan_api_version {VK_API_VERSION_1_0};

        // validation Layers
        bool m_enable_validation_Layers{ true };
        const std::vector<char const*> m_validation_layers {"VK_LAYER_KHRONOS_validation"};
        VkDebugUtilsMessengerEXT m_debug_messenger = nullptr;
        
        // swapchain
        std::vector<char const*> m_device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        VkViewport m_viewport;
        VkRect2D m_scissor;

        // depth test
        VkFormat m_depth_image_format{ VK_FORMAT_UNDEFINED };

        // queue Family Indices
        QueueFamilyIndices m_queue_indices;

        // point light shadow
        bool m_enable_point_light_shadow{ true };

        // used in descriptor pool creation
        uint32_t m_max_vertex_blending_mesh_count{ 256 };
        uint32_t m_max_material_count{ 256 };

        // semaphore and fence
        VkSemaphore m_image_available_for_render_semaphores[k_max_frames_in_flight];
        VkSemaphore m_image_finished_for_presentation_semaphores[k_max_frames_in_flight];
        VkSemaphore m_image_available_for_texturescopy_semaphores[k_max_frames_in_flight];
        VkFence m_is_frame_in_flight_fences[k_max_frames_in_flight];

        // asset allocator use VMA library
        VmaAllocator m_assets_allocator;

    private:
        // auxiliary function
        // validation Layers
        bool checkValidationLayerSupport(); ///< Verification and validation layer support
        std::vector<const char*> getRequiredExtensions(); ///< get Required Extensions, The requirements of glfw and the validation layer
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo); ///< Create the VkDebugUtilsMessengerCreateInfoEXT structure
        VkResult createDebugUtilsMessengerEXT(VkInstance                                instance,
                                            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                            const VkAllocationCallbacks*              pAllocator,
                                            VkDebugUtilsMessengerEXT*                 pDebugMessenger);  ///< Create DebugUtilsMessenger
        void destroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                            VkDebugUtilsMessengerEXT     debugMessenger,
                                            const VkAllocationCallbacks* pAllocator); ///< destroy DebugUtilsMessenger

        // physical device
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physical_device); ///< find Queue Families index
        bool checkDeviceExtensionSupport(VkPhysicalDevice physical_device); ///< check Device Extension Support, Is there any expansion of the swapchain
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physical_device); ///< check SwapChain Support
        bool isDeviceSuitable(VkPhysicalDevice physical_device); ///< Select the appropriate equipment

        // image format
        VkFormat findDepthFormat(); ///< find Supported Depth Format
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
                                     VkImageTiling                tiling,
                                     VkFormatFeatureFlags         features); ///< find Supported Format
    
    private:
        // Initialization process
        void createInstance();
        void initializeDebugMessenger();
        void createWindowSurface();
        void initializePhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();
        void createCommandBuffers();
        void createDescriptorPool();
        void createSyncPrimitives();
        void createAssetAllocator();
    };
}
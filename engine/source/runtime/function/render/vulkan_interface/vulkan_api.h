#pragma once

#include "runtime/function/render/render_type.h"
#include "runtime/function/render/vulkan_interface/vulkan_struct.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <functional>
#include <map>
#include <memory>
#include <vector>

/**
 * Vulkan interface
 */
namespace VKernel
{

    class WindowSystem;

    class VulkanAPI ///< vulkan API
    {

    public:
        ~VulkanAPI();                                                 ///< Destructor
        void initialize(std::shared_ptr<WindowSystem> window_system); ///< init
        void clear();                                                 ///< destory

        void prepareContext(); ///< prepare Context，update m_current_command_buffer

        // command
        bool            prepareBeforePass(std::function<void()> passUpdateAfterRecreateSwapchain);
        void            submitRendering(std::function<void()> passUpdateAfterRecreateSwapchain);
        VkCommandBuffer beginSingleTimeCommands();
        void            endSingleTimeCommands(VkCommandBuffer command_buffer);

        // create(call vulkanUtil)
        VkShaderModule createShaderModule(const std::vector<unsigned char>& shader_code);
        void           createBuffer(VkDeviceSize          size,
                                    VkBufferUsageFlags    usage,
                                    VkMemoryPropertyFlags properties,
                                    VkBuffer&             buffer,
                                    VkDeviceMemory&       buffer_memory);
        void           recreateSwapchain();
        void           copyBuffer(VkBuffer     srcBuffer,
                                  VkBuffer     dstBuffer,
                                  VkDeviceSize srcOffset,
                                  VkDeviceSize dstOffset,
                                  VkDeviceSize size);
        VkSampler      getOrCreateDefaultSampler(DefaultSamplerType type);
        VkSampler      getOrCreateMipmapSampler(uint32_t width, uint32_t height);
        void           createGlobalImage(VkImage&       image,
                                         VkImageView&   image_view,
                                         VmaAllocation& image_allocation,
                                         uint32_t       texture_image_width,
                                         uint32_t       texture_image_height,
                                         void*          texture_image_pixels,
                                         VkFormat       texture_image_format,
                                         uint32_t       miplevels = 0);
        void           createImage(uint32_t              image_width,
                                   uint32_t              image_height,
                                   VkFormat              format,
                                   VkImageTiling         image_tiling,
                                   VkImageUsageFlags     image_usage_flags,
                                   VkMemoryPropertyFlags memory_property_flags,
                                   VkImage&              image,
                                   VkDeviceMemory&       memory,
                                   VkImageCreateFlags    image_create_flags,
                                   uint32_t              array_layers,
                                   uint32_t              miplevels);
        void           createImageView(VkImage            image,
                                       VkFormat           format,
                                       VkImageAspectFlags image_aspect_flags,
                                       VkImageViewType    view_type,
                                       uint32_t           layout_count,
                                       uint32_t           miplevels,
                                       VkImageView&       image_view);

        // query(get)
        VkDevice           getLogicDevice() const;
        SwapChainDesc      getSwapchainInfo();
        void               destroyShaderModule(VkShaderModule shader);
        uint8_t            getMaxFramesInFlight() const;
        VkDescriptorPool   getDescriptorPool() const;
        VkCommandBuffer    getCurrentCommandBuffer() const;
        const VkFence*     getFenceList() const;
        uint8_t            getCurrentFrameIndex() const;
        VkCommandPool      getCommandPool() const;
        uint8_t            getCurrentSwapchainImageIndex() const;
        DepthImageDesc     getDepthImageInfo() const;
        VkPhysicalDevice   getPhysicalDevice() const;
        VmaAllocator       getVmaAllocator() const;
        GLFWwindow*        getWindow() const;
        VkInstance         getInstance() const;
        QueueFamilyIndices getQueueFamilyIndices() const;
        VkQueue            getGraphicsQueue() const;

    private:
        // Maximum parallel frame count
        static uint8_t const k_max_frames_in_flight {2};

    private:
        // external reference
        GLFWwindow* m_window {nullptr};

        // Vulkan object
        VkInstance                    m_instance {nullptr};
        VkSurfaceKHR                  m_surface {nullptr};
        VkPhysicalDevice              m_physical_device {nullptr};
        VkDevice                      m_device {nullptr};
        VkQueue                       m_graphics_queue {nullptr};
        VkQueue                       m_present_queue {nullptr};
        VkCommandPool                 m_command_pool;
        VkCommandPool                 m_api_command_pool;
        VkCommandBuffer               m_command_buffers[k_max_frames_in_flight];
        VkCommandBuffer               m_current_command_buffer {nullptr};
        VkDescriptorPool              m_descriptor_pool {nullptr};
        VkSwapchainKHR                m_swapchain {nullptr};
        std::vector<VkImage>          m_swapchain_images;
        std::vector<VkImageView>      m_swapchain_imageviews;
        VkSemaphore                   m_image_available_for_render_semaphores[k_max_frames_in_flight];
        VkSemaphore                   m_image_finished_for_presentation_semaphores[k_max_frames_in_flight];
        VkFence                       m_is_frame_in_flight_fences[k_max_frames_in_flight];
        VkImageView                   m_depth_image_view {nullptr};
        VkImage                       m_depth_image {nullptr};
        VkDeviceMemory                m_depth_image_memory {nullptr};
        VkSampler                     m_nearest_sampler;
        VkSampler                     m_linear_sampler;
        std::map<uint32_t, VkSampler> m_mipmap_sampler_map;

    private:
        // auxiliary configuration

        // vulkan version
        uint32_t m_vulkan_api_version {VK_API_VERSION_1_0};

        // validation Layers
        bool                           m_enable_validation_Layers {true};
        const std::vector<char const*> m_validation_layers {"VK_LAYER_KHRONOS_validation"};
        VkDebugUtilsMessengerEXT       m_debug_messenger = nullptr;

        // swapchain
        std::vector<char const*> m_device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        VkFormat                 m_swapchain_image_format {VK_FORMAT_UNDEFINED};
        VkExtent2D               m_swapchain_extent;
        VkViewport               m_viewport;
        VkRect2D                 m_scissor;
        uint32_t                 m_current_swapchain_image_index;

        // queue Family Indices
        QueueFamilyIndices m_queue_indices;

        // point light shadow
        bool m_enable_point_light_shadow {true};

        // used in descriptor pool creation
        uint32_t m_max_vertex_blending_mesh_count {256};
        uint32_t m_max_material_count {256};

        // asset allocator use VMA library
        VmaAllocator m_assets_allocator;

        // cur frame index
        uint8_t m_current_frame_index {0};

        // depth
        VkFormat m_depth_image_format {VK_FORMAT_UNDEFINED};

    private:
        // util function

        // validation Layers
        bool checkValidationLayerSupport(); ///< Verification and validation layer support
        std::vector<const char*>
        getRequiredExtensions(); ///< get Required Extensions, The requirements of glfw and the validation layer
        void
        populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&
                                             createInfo); ///< Create the VkDebugUtilsMessengerCreateInfoEXT structure
        VkResult
             createDebugUtilsMessengerEXT(VkInstance                                instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks*              pAllocator,
                                          VkDebugUtilsMessengerEXT* pDebugMessenger); ///< Create DebugUtilsMessenger
        void destroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                           VkDebugUtilsMessengerEXT     debugMessenger,
                                           const VkAllocationCallbacks* pAllocator); ///< destroy DebugUtilsMessenger

        // physical device
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physical_device); ///< find Queue Families index
        bool checkDeviceExtensionSupport(VkPhysicalDevice physical_device);     ///< check Device Extension Support, Is
                                                                            ///< there any expansion of the swapchain
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physical_device); ///< check SwapChain Support
        bool isDeviceSuitable(VkPhysicalDevice physical_device); ///< Select the appropriate equipment

        // image format
        VkFormat findDepthFormat(); ///< find Supported Depth Format
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
                                     VkImageTiling                tiling,
                                     VkFormatFeatureFlags         features); ///< find Supported Format

        // swapchain, choose the best
        VkSurfaceFormatKHR chooseSwapchainSurfaceFormatFromDetails(
            const std::vector<VkSurfaceFormatKHR>& available_surface_formats); ///< Format
        VkPresentModeKHR chooseSwapchainPresentModeFromDetails(
            const std::vector<VkPresentModeKHR>& available_present_modes);                         ///< PresentMode
        VkExtent2D chooseSwapchainExtentFromDetails(const VkSurfaceCapabilitiesKHR& capabilities); ///< Extent

        // depth
        void destroyImageView(VkImageView imageView);

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
        void createSwapchain();
        void createSwapchainImageViews();
        void createFramebufferImageAndView(); ///< depth
        void createAssetAllocator();
    };
} // namespace VKernel
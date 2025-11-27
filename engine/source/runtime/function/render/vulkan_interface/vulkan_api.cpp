#include "runtime/function/render/vulkan_interface/vulkan_api.h"

#include "runtime/function/render/window_system.h"
#include "runtime/function/render/vulkan_interface/vulkan_util.h"

#include <iostream>
#include <set>
#include <algorithm>

namespace VKernel
{

    VulkanAPI::~VulkanAPI(){}

    void VulkanAPI::initialize(std::shared_ptr<WindowSystem> window_system)
    {
        // window
        m_window = window_system->getWindow();
        std::array<int, 2> window_size = window_system->getWindowSize();
        m_viewport = {0.0f, 0.0f, (float)window_size[0], (float)window_size[1], 0.0f, 1.0f};
        m_scissor  = {{0, 0}, {(uint32_t)window_size[0], (uint32_t)window_size[1]}};

        // vulkan object
        createInstance();

        initializeDebugMessenger();

        createWindowSurface();

        initializePhysicalDevice();

        createLogicalDevice();

        createCommandPool();

        createCommandBuffers();

        createDescriptorPool();

        createSyncPrimitives();

        createSwapchain();

        createSwapchainImageViews();

        createAssetAllocator();
    }

    void VulkanAPI::clear()
    {
        if (m_enable_validation_Layers)
        {
            destroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
        }
    }

    void VulkanAPI::prepareContext()
    {
        m_current_command_buffer = m_command_buffers[m_current_frame_index];
    }

    bool VulkanAPI::prepareBeforePass(std::function<void()> passUpdateAfterRecreateSwapchain)
    {
       // acquire image 
       VkResult acquire_image_result =
            vkAcquireNextImageKHR(m_device,
                                m_swapchain,
                                UINT64_MAX,
                                m_image_available_for_render_semaphores[m_current_frame_index],
                                VK_NULL_HANDLE,
                                &m_current_swapchain_image_index);
        

        // whether need recreate swapchain
        if (VK_ERROR_OUT_OF_DATE_KHR == acquire_image_result)
        {
            recreateSwapchain();
            passUpdateAfterRecreateSwapchain(); ///< destory and recreate framebuffer
            return true;
        }
        else if (acquire_image_result != VK_SUCCESS && acquire_image_result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("vkAcquireNextImageKHR failed!");
        }

        // reset fence
        vkResetFences(m_device, 1, &m_is_frame_in_flight_fences[m_current_frame_index]);

        // begin command buffer
        VkCommandBufferBeginInfo command_buffer_begin_info {};
        command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags            = 0;
        command_buffer_begin_info.pInheritanceInfo = nullptr;

        VkResult res_begin_command_buffer = vkBeginCommandBuffer(m_command_buffers[m_current_frame_index], &command_buffer_begin_info);

        if (VK_SUCCESS != res_begin_command_buffer)
        {
            throw std::runtime_error("_vkBeginCommandBuffer failed!");
            return false;
        }
        return false;
    }

    void VulkanAPI::submitRendering(std::function<void()> passUpdateAfterRecreateSwapchain)
    {
        // end command buffer
        VkResult res_end_command_buffer = vkEndCommandBuffer(m_command_buffers[m_current_frame_index]);
        if (VK_SUCCESS != res_end_command_buffer)
        {
            throw std::runtime_error("_vkEndCommandBuffer failed!");
            return;
        }

        // submit command buffer
        VkSemaphore semaphores[1] = {m_image_finished_for_presentation_semaphores[m_current_frame_index]};
        VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSubmitInfo submit_info = {};
        submit_info.sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount     = 1;
        submit_info.pWaitSemaphores        = &m_image_available_for_render_semaphores[m_current_frame_index]; ///< Wait Semaphores  
        submit_info.pWaitDstStageMask      = wait_stages;
        submit_info.commandBufferCount     = 1;
        submit_info.pCommandBuffers        = &m_command_buffers[m_current_frame_index];
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = semaphores; ///< Signal Semaphores

        VkResult res_queue_submit =
        vkQueueSubmit(m_graphics_queue, 1, &submit_info, m_is_frame_in_flight_fences[m_current_frame_index]); ///< Queue Submit
        
        if (VK_SUCCESS != res_queue_submit)
        {
            throw std::runtime_error("vkQueueSubmit failed!");
            return;
        }

        // present swapchain
        VkPresentInfoKHR present_info   = {};
        present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores    = &m_image_finished_for_presentation_semaphores[m_current_frame_index];
        present_info.swapchainCount     = 1;
        present_info.pSwapchains        = &m_swapchain;
        present_info.pImageIndices      = &m_current_swapchain_image_index;
        
        VkResult present_result = vkQueuePresentKHR(m_present_queue, &present_info); ///< Queue Present
        
        // whether need recreate swapchain
        if (VK_ERROR_OUT_OF_DATE_KHR == present_result || VK_SUBOPTIMAL_KHR == present_result)
        {
            recreateSwapchain();
            passUpdateAfterRecreateSwapchain(); ///< destory and recreate framebuffer
        }
        else if (VK_SUCCESS != present_result)
        {
            throw std::runtime_error("vkQueuePresentKHR failed!");
        }

        // update frame index
        m_current_frame_index = (m_current_frame_index + 1) % k_max_frames_in_flight;
    }

    VkShaderModule VulkanAPI::createShaderModule(const std::vector<unsigned char>& shader_code)
    {
        return VulkanUtil::createShaderModule(m_device, shader_code);
    }

    void VulkanAPI::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, 
                                VkBuffer& buffer, VkDeviceMemory& buffer_memory)
    {
        VulkanUtil::createBuffer(m_physical_device, m_device, size, usage, properties, buffer, buffer_memory);
    }

    void VulkanAPI::recreateSwapchain()
    {
        // get glfw framebuffer size
        int width  = 0;
        int height = 0;
        glfwGetFramebufferSize(m_window, &width, &height);
        while (width == 0 || height == 0) // minimized 0,0, pause for now
        {
            glfwGetFramebufferSize(m_window, &width, &height);
            glfwWaitEvents();
        }

        // destory (imageview, swapchain)
        for (auto imageview : m_swapchain_imageviews)
        {
            vkDestroyImageView(m_device, imageview, NULL);
        }
        vkDestroySwapchainKHR(m_device, m_swapchain, NULL);

        // recreate (imageview, swapchain)
        createSwapchain();
        createSwapchainImageViews();
    }

    void VulkanAPI::createSwapchain()
    {
        // query all supports of this physical device
        SwapChainSupportDetails swapchain_support_details = querySwapChainSupport(m_physical_device);

        // choose the best or fitting format
        VkSurfaceFormatKHR chosen_surface_format =
            chooseSwapchainSurfaceFormatFromDetails(swapchain_support_details.formats);
        // choose the best or fitting present mode
        VkPresentModeKHR chosen_presentMode =
            chooseSwapchainPresentModeFromDetails(swapchain_support_details.presentModes);
        // choose the best or fitting extent
        VkExtent2D chosen_extent = chooseSwapchainExtentFromDetails(swapchain_support_details.capabilities);
        // choose image count
        uint32_t image_count = swapchain_support_details.capabilities.minImageCount + 1;
        if (swapchain_support_details.capabilities.maxImageCount > 0 &&
            image_count > swapchain_support_details.capabilities.maxImageCount)
        {
            image_count = swapchain_support_details.capabilities.maxImageCount;
        }

        // Create Swapchain
        VkSwapchainCreateInfoKHR createInfo {};
        createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;

        createInfo.minImageCount    = image_count;
        createInfo.imageFormat      = chosen_surface_format.format;
        createInfo.imageColorSpace  = chosen_surface_format.colorSpace;
        createInfo.imageExtent      = chosen_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = {m_queue_indices.graphics_family.value(), m_queue_indices.present_family.value()};

        if (m_queue_indices.graphics_family != m_queue_indices.present_family)
        {
            createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices   = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform   = swapchain_support_details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode    = chosen_presentMode;
        createInfo.clipped        = VK_TRUE;

        if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS)
        {
            throw std::runtime_error("vk create swapchain khr");
        }

        // Get Swapchain Images
        vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr);
        m_swapchain_images.resize(image_count);
        vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, m_swapchain_images.data());

        m_swapchain_image_format = chosen_surface_format.format;
        m_swapchain_extent.height = chosen_extent.height;
        m_swapchain_extent.width = chosen_extent.width;

        m_scissor = {{0, 0}, {m_swapchain_extent.width, m_swapchain_extent.height}};
    }

    void VulkanAPI::createSwapchainImageViews()
    {
        m_swapchain_imageviews.resize(m_swapchain_images.size());

        // create imageview (one for each this time) for all swapchain images
        for (size_t i = 0; i < m_swapchain_images.size(); i++)
        {
            VkImageView vk_image_view;
            vk_image_view = VulkanUtil::createImageView(m_device,
                                                        m_swapchain_images[i],
                                                        m_swapchain_image_format,
                                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                                        VK_IMAGE_VIEW_TYPE_2D,
                                                        1,
                                                        1);
            m_swapchain_imageviews[i] = vk_image_view;
        }
    }

    VkDevice VulkanAPI::getLogicDevice() const
    {
        return m_device;
    }

    SwapChainDesc VulkanAPI::getSwapchainInfo()
    {
        SwapChainDesc desc;
        desc.image_format = m_swapchain_image_format;
        desc.extent = m_swapchain_extent;
        desc.viewport = m_viewport;
        desc.scissor = m_scissor;
        desc.imageViews = m_swapchain_imageviews;
        return desc;
    }

    void VulkanAPI::destroyShaderModule(VkShaderModule shaderModule)
    {
        vkDestroyShaderModule(m_device, shaderModule, nullptr);
    }

    uint8_t VulkanAPI::getMaxFramesInFlight() const
    {
        return k_max_frames_in_flight;
    }

    VkDescriptorPool VulkanAPI::getDescriptorPool() const
    {
        return m_descriptor_pool;
    }

    VkCommandBuffer VulkanAPI::getCurrentCommandBuffer() const
    {
        return m_current_command_buffer;
    }
    
    const VkFence* VulkanAPI::getFenceList() const
    {
        return m_is_frame_in_flight_fences;
    }
    
    uint8_t VulkanAPI::getCurrentFrameIndex() const
    {
        return m_current_frame_index;
    }
    
    VkCommandPool VulkanAPI::getCommandPool() const
    {
        return m_command_pool;
    }

    uint8_t VulkanAPI::getCurrentSwapchainImageIndex() const
    {
        return m_current_swapchain_image_index;
    }

    bool VulkanAPI::checkValidationLayerSupport()
    {
        // Enumerate Instance Layer Properties
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_validation_layers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0) ///< If the validation layer is found
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> VulkanAPI::getRequiredExtensions()
    {
        // The requirements of glfw and the validation layer
        uint32_t     glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (m_enable_validation_Layers)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT,
                                                        VkDebugUtilsMessageTypeFlagsEXT,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void*) ///< debug callback
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    void VulkanAPI::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        // DebugUtilsMessenger Create Info
        createInfo       = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    VkResult VulkanAPI::createDebugUtilsMessengerEXT(VkInstance                                instance,
                                                     const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                     const VkAllocationCallbacks*              pAllocator,
                                                     VkDebugUtilsMessengerEXT*                 pDebugMessenger)
    {
        // Create DebugUtilsMessenger
        auto func =
            (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VulkanAPI::destroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                                  VkDebugUtilsMessengerEXT     debugMessenger,
                                                  const VkAllocationCallbacks* pAllocator)
    {
        // destroy DebugUtilsMessenger
        auto func =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

    QueueFamilyIndices VulkanAPI::findQueueFamilies(VkPhysicalDevice physicalm_device)
    {
        // get QueueFamily Properties
        QueueFamilyIndices indices;
        uint32_t           queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalm_device, &queue_family_count, nullptr);
        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalm_device, &queue_family_count, queue_families.data());

        // whether support or not graphics command queue, compute command queue and surface presentation
        int i = 0;
        for (const auto& queue_family : queue_families)
        {
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            {
                indices.graphics_family = i;
            }

            VkBool32 is_present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalm_device,
                                                 i,
                                                 m_surface,
                                                 &is_present_support);
            if (is_present_support)
            {
                indices.present_family = i;
            }

            if (indices.isComplete())
            {
                break;
            }
            i++;
        }
        return indices;
    }

    bool VulkanAPI::checkDeviceExtensionSupport(VkPhysicalDevice physicalm_device)
    {
        // Enumerate Device Extension Properties
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(physicalm_device, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(physicalm_device, nullptr, &extension_count, available_extensions.data());

        // If the swapchain expansion returns true
        std::set<std::string> required_extensions(m_device_extensions.begin(), m_device_extensions.end());
        for (const auto& extension : available_extensions)
        {
            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    }

    SwapChainSupportDetails VulkanAPI::querySwapChainSupport(VkPhysicalDevice physicalm_device)
    {
        SwapChainSupportDetails details_result;

        // capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalm_device, m_surface, &details_result.capabilities);

        // formats
        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalm_device, m_surface, &format_count, nullptr);
        if (format_count != 0)
        {
            details_result.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                physicalm_device, m_surface, &format_count, details_result.formats.data());
        }

        // present modes
        uint32_t presentmode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalm_device, m_surface, &presentmode_count, nullptr);
        if (presentmode_count != 0)
        {
            details_result.presentModes.resize(presentmode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                physicalm_device, m_surface, &presentmode_count, details_result.presentModes.data());
        }

        return details_result;
    }

    bool VulkanAPI::isDeviceSuitable(VkPhysicalDevice physicalm_device)
    {
        // If the queue family, the exchange chain, and the sampler all support, return true.
        auto queue_indices           = findQueueFamilies(physicalm_device);
        bool is_extensions_supported = checkDeviceExtensionSupport(physicalm_device);
        bool is_swapchain_adequate   = false;
        if (is_extensions_supported)
        {
            SwapChainSupportDetails swapchain_support_details = querySwapChainSupport(physicalm_device);
            is_swapchain_adequate =
                !swapchain_support_details.formats.empty() && !swapchain_support_details.presentModes.empty();
        }

        if (!queue_indices.isComplete() || !is_swapchain_adequate)
        {
            return false;
        }

        return true;
    }

    VkFormat VulkanAPI::findDepthFormat()
    {
        return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                   VK_IMAGE_TILING_OPTIMAL,
                                   VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    VkFormat VulkanAPI::findSupportedFormat(const std::vector<VkFormat>& candidates,
                                            VkImageTiling                tiling,
                                            VkFormatFeatureFlags         features)
    {
        for (VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_physical_device, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        throw std::runtime_error("findSupportedFormat failed");
        return VkFormat();
    }

    VkSurfaceFormatKHR
    VulkanAPI::chooseSwapchainSurfaceFormatFromDetails(const std::vector<VkSurfaceFormatKHR>& available_surface_formats)
    {
        for (const auto& surface_format : available_surface_formats)
        {
            // select the VK_FORMAT_B8G8R8A8_SRGB surface format,
            // there is no need to do gamma correction in the fragment shader
            if (surface_format.format == VK_FORMAT_B8G8R8A8_UNORM &&
                surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return surface_format;
            }
        }
        return available_surface_formats[0];
    }

    VkPresentModeKHR
    VulkanAPI::chooseSwapchainPresentModeFromDetails(const std::vector<VkPresentModeKHR>& available_present_modes)
    {
        // select the VK_PRESENT_MODE_MAILBOX_KHR
        for (VkPresentModeKHR present_mode : available_present_modes)
        {
            if (VK_PRESENT_MODE_MAILBOX_KHR == present_mode)
            {
                return VK_PRESENT_MODE_MAILBOX_KHR;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanAPI::chooseSwapchainExtentFromDetails(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX) ///< Not resizable
        {
            return capabilities.currentExtent;
        }
        else ///< Restore the window size and ensure it is within the supported range of the function.
        {
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);

            VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

            actualExtent.width =
                std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height =
                std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanAPI::createInstance()
    {
        // Verification and validation layer support
        if (m_enable_validation_Layers && !checkValidationLayerSupport())
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        // app info
        
        m_vulkan_api_version = VK_API_VERSION_1_0;
        VkApplicationInfo appInfo {};
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName   = "vkernel_renderer";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName        = "VKernel";
        appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion         = m_vulkan_api_version;

        // create info
        VkInstanceCreateInfo instance_create_info {};
        instance_create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pApplicationInfo = &appInfo;

        // extensions
        auto extensions                              = getRequiredExtensions();
        instance_create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
        instance_create_info.ppEnabledExtensionNames = extensions.data();

        // Layer and expansion structure
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};
        if (m_enable_validation_Layers)
        {
            instance_create_info.enabledLayerCount   = static_cast<uint32_t>(m_validation_layers.size());
            instance_create_info.ppEnabledLayerNames = m_validation_layers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            instance_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else
        {
            instance_create_info.enabledLayerCount = 0;
            instance_create_info.pNext             = nullptr;
        }

        // create instance
        if (vkCreateInstance(&instance_create_info, nullptr, &m_instance) != VK_SUCCESS)
        {
            throw std::runtime_error("vk create instance");
        }
    }

    void VulkanAPI::initializeDebugMessenger()
    {
        if (m_enable_validation_Layers)
        {
            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            populateDebugMessengerCreateInfo(createInfo);
            if (VK_SUCCESS != createDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debug_messenger))
            {
                throw std::runtime_error("failed to set up debug messenger!");
            }
        }
    }

    void VulkanAPI::createWindowSurface()
    {
        // create surface
        if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            throw std::runtime_error("glfwCreateWindowSurface failed!");
        }
    }

    void VulkanAPI::initializePhysicalDevice()
    {
        // Find all physical devices through instance
        uint32_t physical_device_count;
        vkEnumeratePhysicalDevices(m_instance, &physical_device_count, nullptr);
        if (physical_device_count == 0)
        {
            throw std::runtime_error("enumerate physical devices failed!");
        }
        else
        {
            std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
            vkEnumeratePhysicalDevices(m_instance, &physical_device_count, physical_devices.data());

            // Select the appropriate equipment
            for (const auto& device : physical_devices)
            {
                if (isDeviceSuitable(device))
                {
                    m_physical_device = device;
                    break;
                }
            }

            if (m_physical_device == VK_NULL_HANDLE)
            {
                throw std::runtime_error("failed to find suitable physical device");
            }
        }
    }

    void VulkanAPI::createLogicalDevice()
    {
        // queue create info
        m_queue_indices = findQueueFamilies(m_physical_device);

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<uint32_t> queue_families = {m_queue_indices.graphics_family.value(),
                                             m_queue_indices.present_family.value()};

        float queue_priority = 1.0f;
        for (uint32_t queue_family : queue_families) ///< for every queue family
        {
            
            VkDeviceQueueCreateInfo queue_create_info {};
            queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.queueCount       = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        // device create info
        VkPhysicalDeviceFeatures physical_device_features = {};
        VkDeviceCreateInfo device_create_info {};
        device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pQueueCreateInfos       = queue_create_infos.data();
        device_create_info.queueCreateInfoCount    = static_cast<uint32_t>(queue_create_infos.size());
        device_create_info.pEnabledFeatures        = &physical_device_features;
        device_create_info.enabledExtensionCount   = static_cast<uint32_t>(m_device_extensions.size());
        device_create_info.ppEnabledExtensionNames = m_device_extensions.data();
        if (m_enable_validation_Layers) {
            device_create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
            device_create_info.ppEnabledLayerNames = m_validation_layers.data();
        } else {
            device_create_info.enabledLayerCount = 0;
        }

        // create device
        if (vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_device) != VK_SUCCESS)
        {
            throw std::runtime_error("vk create device");
        }

        // Obtain the queues of this device
        vkGetDeviceQueue(m_device, m_queue_indices.graphics_family.value(), 0, &m_graphics_queue);
        vkGetDeviceQueue(m_device, m_queue_indices.present_family.value(), 0, &m_present_queue);
    }
    
    void VulkanAPI::createCommandPool()
    {
        // create command pools
        {
            VkCommandPoolCreateInfo command_pool_create_info;
            command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            command_pool_create_info.pNext            = NULL;
            command_pool_create_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; ///< The command buffer will have a short lifespan.
            command_pool_create_info.queueFamilyIndex = m_queue_indices.graphics_family.value();

            if (vkCreateCommandPool(m_device, &command_pool_create_info, NULL, &m_command_pool) != VK_SUCCESS)
            {
                throw std::runtime_error("vk create command pool");
            }
        }
    }

    void VulkanAPI::createCommandBuffers()
    {
        // Allocate command pools
        VkCommandBufferAllocateInfo command_buffer_allocate_info {};
        command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_allocate_info.commandPool = m_command_pool;
        command_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_allocate_info.commandBufferCount = sizeof(m_command_buffers) / sizeof(m_command_buffers[0]);

        if (vkAllocateCommandBuffers(m_device, &command_buffer_allocate_info, m_command_buffers) != VK_SUCCESS)
        {
            throw std::runtime_error("vk allocate command buffers");
        }
    }

    void VulkanAPI::createDescriptorPool()
    {
        // The descriptor memory pool pre-allocates a certain number of various types of 
        // descriptors to avoid frequent creation and destruction during runtime.

        // create Descriptor pools
        VkDescriptorPoolSize pool_sizes[7];
        pool_sizes[0].type            = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC; ///< ssbo, Can be dynamically shifted
        pool_sizes[0].descriptorCount = 3 + 2 + 2 + 2 + 1 + 1 + 3 + 3;
        pool_sizes[1].type            = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; ///< ssbo
        pool_sizes[1].descriptorCount = 1 + 1 + 1 * m_max_vertex_blending_mesh_count;
        pool_sizes[2].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; ///< ubo
        pool_sizes[2].descriptorCount = 1 * m_max_material_count;
        pool_sizes[3].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; ///< Image + Sampler combination
        pool_sizes[3].descriptorCount = 3 + 5 * m_max_material_count + 1 + 1;
        pool_sizes[4].type            = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; ///< Frame buffer attachment reading
        pool_sizes[4].descriptorCount = 4 + 1 + 1 + 2;
        pool_sizes[5].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC; ///< ubo, Can be dynamically shifted
        pool_sizes[5].descriptorCount = 3;
        pool_sizes[6].type            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE; ///< Readable and writable images
        pool_sizes[6].descriptorCount = 1;

        VkDescriptorPoolCreateInfo pool_info {};
        pool_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.poolSizeCount = sizeof(pool_sizes) / sizeof(pool_sizes[0]);
        pool_info.pPoolSizes    = pool_sizes;
        pool_info.maxSets =
            1 + 1 + 1 + m_max_material_count + m_max_vertex_blending_mesh_count + 1 + 1; ///< Maximum number of sets
        pool_info.flags = 0U;

        if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_descriptor_pool) != VK_SUCCESS)
        {
            throw std::runtime_error("create descriptor pool");
        }
    }

    void VulkanAPI::createSyncPrimitives()
    {
        // creat semaphore and fence
        VkSemaphoreCreateInfo semaphore_create_info {};
        semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_create_info {};
        fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT; // the fence is initialized as signaled

        for (uint32_t i = 0; i < k_max_frames_in_flight; i++)
        {
            if (vkCreateSemaphore(
                    m_device, &semaphore_create_info, nullptr, &m_image_available_for_render_semaphores[i]) !=
                    VK_SUCCESS ||
                vkCreateSemaphore(
                    m_device, &semaphore_create_info, nullptr, &m_image_finished_for_presentation_semaphores[i]) !=
                    VK_SUCCESS ||
                vkCreateFence(m_device, &fence_create_info, nullptr, &m_is_frame_in_flight_fences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("vk create semaphore & fence");
            }
        }
    }

    void VulkanAPI::createAssetAllocator()
    {
        // creat Allocator
        VmaVulkanFunctions vulkanFunctions    = {};
        vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
        vulkanFunctions.vkGetDeviceProcAddr   = &vkGetDeviceProcAddr;

        VmaAllocatorCreateInfo allocatorCreateInfo = {};
        allocatorCreateInfo.vulkanApiVersion       = m_vulkan_api_version;
        allocatorCreateInfo.physicalDevice         = m_physical_device;
        allocatorCreateInfo.device                 = m_device;
        allocatorCreateInfo.instance               = m_instance;
        allocatorCreateInfo.pVulkanFunctions       = &vulkanFunctions;

        vmaCreateAllocator(&allocatorCreateInfo, &m_assets_allocator);
    }
}
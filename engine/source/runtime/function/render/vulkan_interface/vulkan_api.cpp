#include "runtime/function/render/vulkan_interface/vulkan_api.h"

#include "runtime/function/render/window_system.h"

#include <iostream>
#include <set>
#include <algorithm>

namespace VKernel
{

    VulkanAPI::~VulkanAPI(){}

    void VulkanAPI::initialize(std::shared_ptr<WindowSystem> window_system)
    {
        m_window = window_system->getWindow();
        std::array<int, 2> window_size = window_system->getWindowSize();
        m_viewport = {0.0f, 0.0f, (float)window_size[0], (float)window_size[1], 0.0f, 1.0f};
        m_scissor  = {{0, 0}, {(uint32_t)window_size[0], (uint32_t)window_size[1]}};
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

            if (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                indices.m_compute_family = i;
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

        // If the exchange chain expansion returns true
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

        VkPhysicalDeviceFeatures physicalm_device_features;
        vkGetPhysicalDeviceFeatures(physicalm_device, &physicalm_device_features);

        if (!queue_indices.isComplete() || !is_swapchain_adequate || !physicalm_device_features.samplerAnisotropy)
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

        std::cerr << "findSupportedFormat failed" << std::endl;
        return VkFormat();
    }

    void VulkanAPI::createInstance()
    {
        // Verification and validation layer support
        if (m_enable_validation_Layers && !checkValidationLayerSupport())
        {
            std::cerr << "validation layers requested, but not available!" << std::endl;
        }

        m_vulkan_api_version = VK_API_VERSION_1_0;

        // app info
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
            std::cerr << "vk create instance" << std::endl;
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
                std::cerr << "failed to set up debug messenger!" << std::endl;
            }
        }
    }

    void VulkanAPI::createWindowSurface()
    {
        // create surface
        if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            std::cerr << "glfwCreateWindowSurface failed!" << std::endl;
        }
    }

    void VulkanAPI::initializePhysicalDevice()
    {
        // Find all physical devices through instance
        uint32_t physical_device_count;
        vkEnumeratePhysicalDevices(m_instance, &physical_device_count, nullptr);
        if (physical_device_count == 0)
        {
            std::cerr << "enumerate physical devices failed!" << std::endl;
        }
        else
        {
            std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
            vkEnumeratePhysicalDevices(m_instance, &physical_device_count, physical_devices.data());

            // Enumerate Physical Devices, Add to the alternative list
            std::vector<std::pair<int, VkPhysicalDevice>> ranked_physical_devices;
            for (const auto& device : physical_devices)
            {
                VkPhysicalDeviceProperties physical_device_properties;
                vkGetPhysicalDeviceProperties(device, &physical_device_properties);
                int score = 0;

                if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) ///< Independent graphics card
                {
                    score += 1000;
                }
                else if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) ///< integrated graphics card
                {
                    score += 100;
                }

                ranked_physical_devices.push_back({score, device});
            }

            // Sort by scores
            std::sort(ranked_physical_devices.begin(),
                      ranked_physical_devices.end(),
                      [](const std::pair<int, VkPhysicalDevice>& p1, const std::pair<int, VkPhysicalDevice>& p2) {
                          return p1 > p2;
                      });

            // Select the appropriate equipment
            for (const auto& device : ranked_physical_devices)
            {
                if (isDeviceSuitable(device.second))
                {
                    m_physical_device = device.second;
                    break;
                }
            }

            if (m_physical_device == VK_NULL_HANDLE)
            {
                std::cerr << "failed to find suitable physical device" << std::endl;
            }
        }
    }

    void VulkanAPI::createLogicalDevice()
    {
        // queue create info
        m_queue_indices = findQueueFamilies(m_physical_device);

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<uint32_t>                   queue_families = {m_queue_indices.graphics_family.value(),
                                             m_queue_indices.present_family.value(),
                                             m_queue_indices.m_compute_family.value()};

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

        // physical device features
        VkPhysicalDeviceFeatures physical_device_features = {};

        physical_device_features.samplerAnisotropy = VK_TRUE; ///< anti-aliasing

        physical_device_features.fragmentStoresAndAtomics = VK_TRUE; //< support inefficient readback storage buffer

        physical_device_features.independentBlend = VK_TRUE; //< support independent blending
        
        if (m_enable_point_light_shadow)
        {
            physical_device_features.geometryShader = VK_TRUE; //< support geometry shader
        }

        // device create info
        VkDeviceCreateInfo device_create_info {};
        device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pQueueCreateInfos       = queue_create_infos.data();
        device_create_info.queueCreateInfoCount    = static_cast<uint32_t>(queue_create_infos.size());
        device_create_info.pEnabledFeatures        = &physical_device_features;
        device_create_info.enabledExtensionCount   = static_cast<uint32_t>(m_device_extensions.size());
        device_create_info.ppEnabledExtensionNames = m_device_extensions.data();
        device_create_info.enabledLayerCount       = 0;

        // create device
        if (vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_device) != VK_SUCCESS)
        {
            std::cerr << "vk create device" << std::endl;
        }

        // Obtain the queues of this device
        vkGetDeviceQueue(m_device, m_queue_indices.graphics_family.value(), 0, &m_graphics_queue);
        vkGetDeviceQueue(m_device, m_queue_indices.present_family.value(), 0, &m_present_queue);
        vkGetDeviceQueue(m_device, m_queue_indices.m_compute_family.value(), 0, &m_compute_queue);

        // find Supported Depth Format
        m_depth_image_format = (VkFormat)findDepthFormat();
    }
    
    void VulkanAPI::createCommandPool()
    {
        // create command pools
        {
            VkCommandPoolCreateInfo command_pool_create_info;
            command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            command_pool_create_info.pNext            = NULL;
            command_pool_create_info.flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT; ///< The command buffer will have a short lifespan.
            command_pool_create_info.queueFamilyIndex = m_queue_indices.graphics_family.value();

            for (uint32_t i = 0; i < k_max_frames_in_flight; ++i)
            {
                if (vkCreateCommandPool(m_device, &command_pool_create_info, NULL, &m_command_pools[i]) != VK_SUCCESS)
                {
                    std::cerr << "vk create command pool" << std::endl;
                }
            }
        }
    }

    void VulkanAPI::createCommandBuffers()
    {
        // Allocate command pools
        VkCommandBufferAllocateInfo command_buffer_allocate_info {};
        command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_allocate_info.commandBufferCount = 1U;

        for (uint32_t i = 0; i < k_max_frames_in_flight; ++i)
        {
            command_buffer_allocate_info.commandPool = m_command_pools[i];
            VkCommandBuffer vk_command_buffer;
            if (vkAllocateCommandBuffers(m_device, &command_buffer_allocate_info, &vk_command_buffer) != VK_SUCCESS)
            {
                std::cerr << "vk allocate command buffers" << std::endl;
            }
            m_command_buffers[i] = vk_command_buffer;
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
            std::cerr << "create descriptor pool" << std::endl;
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
                vkCreateSemaphore(
                    m_device, &semaphore_create_info, nullptr, &m_image_available_for_texturescopy_semaphores[i]) !=
                    VK_SUCCESS ||
                vkCreateFence(m_device, &fence_create_info, nullptr, &m_is_frame_in_flight_fences[i]) != VK_SUCCESS)
            {
                std::cerr << "vk create semaphore & fence" << std::endl;
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

    void VulkanAPI::clear()
    {
        if (m_enable_validation_Layers)
        {
            destroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
        }
    }
}
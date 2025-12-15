#pragma once

#include "runtime/core/math/vector2.h"
#include "runtime/core/math/vector3.h"
#include "runtime/core/math/vector4.h"

#include "runtime/function/render/vulkan_interface/vulkan_api.h"

#include <array>

/**
 * Vertex data Layout and Descriptions
 */
namespace VKernel
{
    struct MeshVertex
    {
        struct VulkanMeshVertexPostition
        {
            Vector3 position;
        };

        static std::array<VkVertexInputBindingDescription, 1> getBindingDescriptions()
        {
            std::array<VkVertexInputBindingDescription, 1> binding_descriptions {};

            // position
            binding_descriptions[0].binding   = 0;
            binding_descriptions[0].stride    = sizeof(VulkanMeshVertexPostition);
            binding_descriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return binding_descriptions;
        }

        static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 1> attribute_descriptions {};

            // position
            attribute_descriptions[0].binding  = 0;
            attribute_descriptions[0].location = 0;
            attribute_descriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
            attribute_descriptions[0].offset   = offsetof(VulkanMeshVertexPostition, position);

            return attribute_descriptions;
        }
    };
}

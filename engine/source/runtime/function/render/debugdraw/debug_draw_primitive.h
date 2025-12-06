#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"
#include "runtime/core/math/math_headers.h"

#include <array>

/**
 * Basic primitive: Vertex data, vertex description
 */
namespace VKernel
{
    enum DebugDrawPrimitiveType : uint8_t ///< Primitive Type
    {
        _debug_draw_primitive_type_point = 0,
        _debug_draw_primitive_type_line,
        _debug_draw_primitive_type_triangle,
        _debug_draw_primitive_type_quad,
        _debug_draw_primitive_type_draw_box,
        _debug_draw_primitive_type_cylinder,
        _debug_draw_primitive_type_sphere,
        _debug_draw_primitive_type_capsule,
        _debug_draw_primitive_type_text,
        k_debug_draw_primitive_type_count
    };

    struct DebugDrawVertex ///< Vertex data
    {
        // Vertex data
        Vector3 pos;
        Vector4 color;

        DebugDrawVertex() ///< Default initialization Vertex data
        {
            pos = Vector3(-1.0f, -1.0f, -1.0f);
            color = Vector4(-1.0f, -1.0f, -1.0f, -1.0f);
        }

        // Vertex Description
        static VkVertexInputBindingDescription getBindingDescriptions()
        {
            VkVertexInputBindingDescription binding_descriptions;
            binding_descriptions.binding = 0;
            binding_descriptions.stride = sizeof(DebugDrawVertex);
            binding_descriptions.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; ///< vertex index or instance index

            return binding_descriptions;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions{};

            // position
            attribute_descriptions[0].binding = 0;
            attribute_descriptions[0].location = 0;
            attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attribute_descriptions[0].offset = offsetof(DebugDrawVertex, pos);

            // color
            attribute_descriptions[1].binding = 0;
            attribute_descriptions[1].location = 1;
            attribute_descriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attribute_descriptions[1].offset = offsetof(DebugDrawVertex, color);

            return attribute_descriptions;
        }
    };

    class DebugDrawPrimitive ///< Primitive base class
    {
    public:
        static constexpr int BASIC_COUNT = 10; ///< basic vertex count
    };

    class DebugDrawTriangle : public DebugDrawPrimitive ///< Triangle Primitive derived class
    {
    public:
        DebugDrawVertex m_vertex[3]; ///< Vertex

        uint8_t m_indices[3]; ///< indice

        Transform m_model; ///< model

        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_triangle; ///< Primitive Type
    };

    class DebugDrawQuad : public DebugDrawPrimitive ///< quad
    {
    public:
        DebugDrawVertex m_vertex[4];

        uint8_t m_indices[6];

        Transform m_model;

        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_quad;
    };

    class DebugDrawBox : public DebugDrawPrimitive ///< box
    {
    public:
        DebugDrawVertex m_vertex[8];

        uint8_t m_indices[36];

        Transform m_model;

        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_draw_box;
    };

    class DebugDrawSphere : public DebugDrawPrimitive
    {
    public:
        static constexpr int SPHERE_BASIC_COUNT = (BASIC_COUNT * 2 + 2) * (BASIC_COUNT * 2) * 2 + (BASIC_COUNT * 2 + 1) * (BASIC_COUNT * 2) * 2;

        DebugDrawVertex m_vertex[SPHERE_BASIC_COUNT];
        Transform m_model;

        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_sphere;
    };
}
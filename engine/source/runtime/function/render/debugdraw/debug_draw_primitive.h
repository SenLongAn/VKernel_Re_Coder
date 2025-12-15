#pragma once

#include "runtime/function/render/vulkan_interface/vulkan_api.h"
#include "runtime/core/math/math_headers.h"

#include <array>

/**
 * Basic mesh
 */
namespace VKernel
{
    enum DebugDrawPrimitiveType : uint8_t ///< Basic mesh Type
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

    enum PrimitiveType : uint8_t ///< primitive type
    {
        _Primitive_point = 0,
        _Primitive_line = 1,
        _Primitive_triangle = 2,
        k_Primitive_count,
    };

    enum TextureType : uint32_t ///< texture type
    {
        UNDEFINED = 0,
        GOLD = 1,
        FLOOR = 2,
        WOOD = 3,
        BLOCK = 4,
        BRICKWALL = 5,
        CONTAINER = 6,
        TEXTURE_TYPE_COUNT = 7
    };

    // Vertex data Layout and Descriptions
    struct DebugDrawVertex
    {
        // Vertex data
        Vector3 pos;
        Vector2 texcoord;

        DebugDrawVertex() ///< Default initialization Vertex data
        {
            pos = Vector3(-1.0f, -1.0f, -1.0f);
            texcoord = Vector2(-1.0f, -1.0f);
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

            // texcoord
            attribute_descriptions[1].binding = 0;
            attribute_descriptions[1].location = 1;
            attribute_descriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
            attribute_descriptions[1].offset = offsetof(DebugDrawVertex, texcoord);

            return attribute_descriptions;
        }
    };

    class DebugDrawPrimitive ///< Basic mesh _base class
    {
    public:
        PrimitiveType m_primitive_type{k_Primitive_count}; ///< primitive type

        bool m_is_depth_test = false; ///< Is there a depth test

        Vector4 m_color; ///< color

        Transform m_model; ///< model

        TextureType m_texture_type; ///< texture type
    };

    class DebugDrawTriangle : public DebugDrawPrimitive ///< Triangle Primitive derived class
    {
    public:
        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_triangle; ///< Primitive Type
    };

    class DebugDrawQuad : public DebugDrawPrimitive ///< quad
    {
    public:
        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_quad;
    };

    class DebugDrawBox : public DebugDrawPrimitive ///< box
    {
    public:
        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_draw_box;
    };

    class DebugDrawSphere : public DebugDrawPrimitive
    {
    public:
        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_sphere;
    };

    class DebugDrawCylinder : public DebugDrawPrimitive
    {
    public:
        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_cylinder;
    };

    class DebugDrawCapsule : public DebugDrawPrimitive
    {
    public:
        static const DebugDrawPrimitiveType k_type_enum_value = _debug_draw_primitive_type_capsule;
    };
}
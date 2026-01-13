#pragma once

#include "runtime/core/base/hash.h"

#include <cstdint>
#include <memory>
#include <string>

#include <vulkan/vulkan.h>

namespace VKernel
{

    enum class RENDER_PIPELINE_TYPE : uint8_t ///< render mode
    {
        FORWARD_PIPELINE = 0,
        DEFERRED_PIPELINE,
        PIPELINE_TYPE_COUNT
    };

    // mesh
    class BufferData ///< data
    {
    public:
        size_t m_size {0};       ///< size
        void*  m_data {nullptr}; ///< data

        BufferData() = delete;
        BufferData(size_t size)
        {
            m_size = size;
            m_data = malloc(size);
        }
        ~BufferData()
        {
            if (m_data)
            {
                free(m_data);
            }
        }
        bool isValid() const { return m_data != nullptr; }
    };

    struct StaticMeshData ///< mesh data
    {
        std::shared_ptr<BufferData> m_vertex_buffer; ///< vertex
        std::shared_ptr<BufferData> m_index_buffer;  ///< index
    };

    struct RenderMeshData ///< mesh data
    {
        StaticMeshData m_static_mesh_data;
    };

    struct MeshSourceDesc ///< mesh file path
    {
        std::string m_mesh_file; ///< mesh file path

        bool   operator==(const MeshSourceDesc& rhs) const { return m_mesh_file == rhs.m_mesh_file; } ///< is equal
        size_t getHashValue() const { return std::hash<std::string> {}(m_mesh_file); }                ///< is has value
    };

    struct MeshVertexDataDefinition ///< vertex data struct
    {
        float x, y, z;    // position
        float nx, ny, nz; // normal
        float tx, ty, tz; // tangent
        float u, v;       // UV coordinates
    };

    // Material

    enum class IMAGE_TYPE : uint8_t ///< image type
    {
        IMAGE_TYPE_UNKNOWM = 0,
        IMAGE_TYPE_2D
    };

    enum DefaultSamplerType ///< image sampler type
    {
        Default_Sampler_Linear,
        Default_Sampler_Nearest
    };
    class TextureData ///< texture data
    {
    public:
        uint32_t m_width {0};
        uint32_t m_height {0};
        uint32_t m_depth {0};
        uint32_t m_mip_levels {0};
        uint32_t m_array_layers {0};
        void*    m_pixels {nullptr}; ///< data

        VkFormat   m_format = VK_FORMAT_MAX_ENUM;
        IMAGE_TYPE m_type {IMAGE_TYPE::IMAGE_TYPE_UNKNOWM};

        TextureData() = default;
        ~TextureData()
        {
            if (m_pixels)
            {
                free(m_pixels);
            }
        }
        bool isValid() const { return m_pixels != nullptr; }
    };
    struct MaterialSourceDesc ///< Material file path
    {
        std::string m_base_color_file;
        std::string m_normal_file;

        bool operator==(const MaterialSourceDesc& rhs) const
        {
            return m_base_color_file == rhs.m_base_color_file && m_normal_file == rhs.m_normal_file;
        }

        size_t getHashValue() const
        {
            size_t hash = 0;
            hash_combine(hash, m_base_color_file, m_normal_file);
            return hash;
        }
    };

    struct RenderMaterialData ///< Material data
    {
        std::shared_ptr<TextureData> m_base_color_texture;
        std::shared_ptr<TextureData> m_normal_texture;
    };

} // namespace VKernel

template<>
struct std::hash<VKernel::MeshSourceDesc>
{
    size_t operator()(const VKernel::MeshSourceDesc& rhs) const noexcept { return rhs.getHashValue(); }
};

template<>
struct std::hash<VKernel::MaterialSourceDesc>
{
    size_t operator()(const VKernel::MaterialSourceDesc& rhs) const noexcept { return rhs.getHashValue(); }
};
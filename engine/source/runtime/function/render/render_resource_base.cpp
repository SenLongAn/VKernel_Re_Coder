#include "runtime/function/render/render_resource_base.h"

#include "runtime/core/math/vector2.h"
#include "runtime/core/math/vector3.h"
#include "runtime/core/math/vector4.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "render_resource_base.h"
#include "tiny_obj_loader.h"

#include <filesystem>
#include <iostream>

namespace VKernel
{
    std::shared_ptr<TextureData> RenderResourceBase::loadTexture(std::string file, bool is_srgb)
    {
        std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();

        int iw, ih, n;
        texture->m_pixels = stbi_load(file.c_str(), &iw, &ih, &n, 4);

        if (!texture->m_pixels)
            return nullptr;

        texture->m_width        = iw;
        texture->m_height       = ih;
        texture->m_format       = (is_srgb) ? VkFormat::VK_FORMAT_R8G8B8A8_SRGB : VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
        texture->m_depth        = 1;
        texture->m_array_layers = 1;
        texture->m_mip_levels   = 1;
        texture->m_type         = IMAGE_TYPE::IMAGE_TYPE_2D;

        return texture;
    }

    RenderMeshData RenderResourceBase::loadMeshData(const MeshSourceDesc& source)
    {
        RenderMeshData ret;

        if (std::filesystem::path(source.m_mesh_file).extension() == ".obj") ///< The file extension is .obj
        {
            ret.m_static_mesh_data = loadStaticMesh(source.m_mesh_file);
        }

        return ret;
    }

    StaticMeshData RenderResourceBase::loadStaticMesh(std::string mesh_file)
    {
        StaticMeshData mesh_data;

        // tinyobj 3rdparty
        tinyobj::ObjReader       reader;
        tinyobj::ObjReaderConfig reader_config;
        reader_config.vertex_color = false;
        if (!reader.ParseFromFile(mesh_file, reader_config)) ///< parse from mesh file
        {
            if (!reader.Error().empty()) ///< if failed
            {
                throw std::runtime_error("loadMesh failed!");
            }
            assert(0);
        }

        if (!reader.Warning().empty())
        {
            std::cerr << "loadMesh warning!" << std::endl;
        }

        auto& attrib = reader.GetAttrib(); ///< vertex data
        auto& shapes = reader.GetShapes(); ///< all mesh

        // read data, Write to mesh_vertices
        std::vector<MeshVertexDataDefinition> mesh_vertices;

        for (size_t s = 0; s < shapes.size(); s++) ///< Iterate over all sub mesh
        {
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) ///< Iterate over all face
            {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]); ///< face vertex size

                // Data Structure
                bool with_normal   = true;
                bool with_texcoord = true;

                Vector3 vertex[3];
                // Vector3 normal[3];
                // Vector2 uv[3];

                // only deals with triangle faces
                if (fv != 3)
                {
                    continue;
                }

                // Iterate over the 3 vertices of a face
                for (size_t v = 0; v < fv; v++)
                {
                    auto idx = shapes[s].mesh.indices[index_offset + v];

                    // vertex
                    auto vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    auto vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    auto vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                    vertex[v].x = static_cast<float>(vx);
                    vertex[v].y = static_cast<float>(vy);
                    vertex[v].z = static_cast<float>(vz);

                    // // normal
                    // if (idx.normal_index >= 0)
                    // {
                    //     auto nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    //     auto ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    //     auto nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

                    //     normal[v].x = static_cast<float>(nx);
                    //     normal[v].y = static_cast<float>(ny);
                    //     normal[v].z = static_cast<float>(nz);
                    // }
                    // else
                    // {
                    //     with_normal = false;
                    // }

                    // // texcoord
                    // if (idx.texcoord_index >= 0)
                    // {
                    //     auto tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    //     auto ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

                    //     uv[v].x = static_cast<float>(tx);
                    //     uv[v].y = static_cast<float>(ty);
                    // }
                    // else
                    // {
                    //     with_texcoord = false;
                    // }
                }
                index_offset += fv;

                // if (!with_normal) ///< No normal default data set
                // {
                //     Vector3 v0 = vertex[1] - vertex[0];
                //     Vector3 v1 = vertex[2] - vertex[1];
                //     normal[0]  = v0.crossProduct(v1).normalisedCopy();
                //     normal[1]  = normal[0];
                //     normal[2]  = normal[0];
                // }

                // if (!with_texcoord) ///< No texcoord default data set
                // {
                //     uv[0] = Vector2(0.5f, 0.5f);
                //     uv[1] = Vector2(0.5f, 0.5f);
                //     uv[2] = Vector2(0.5f, 0.5f);
                // }

                // Vector3 tangent {1, 0, 0}; ///< Calculate tangent
                // {
                //     Vector3 edge1    = vertex[1] - vertex[0];
                //     Vector3 edge2    = vertex[2] - vertex[1];
                //     Vector2 deltaUV1 = uv[1] - uv[0];
                //     Vector2 deltaUV2 = uv[2] - uv[1];

                //     auto divide = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
                //     if (divide >= 0.0f && divide < 0.000001f)
                //         divide = 0.000001f;
                //     else if (divide < 0.0f && divide > -0.000001f)
                //         divide = -0.000001f;

                //     float df  = 1.0f / divide;
                //     tangent.x = df * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                //     tangent.y = df * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                //     tangent.z = df * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                //     tangent   = (tangent).normalisedCopy();
                // }

                // write data for a triangle primitive
                for (size_t i = 0; i < 3; i++)
                {
                    MeshVertexDataDefinition mesh_vert {};

                    mesh_vert.x = vertex[i].x;
                    mesh_vert.y = vertex[i].y;
                    mesh_vert.z = vertex[i].z;

                    // mesh_vert.nx = normal[i].x;
                    // mesh_vert.ny = normal[i].y;
                    // mesh_vert.nz = normal[i].z;

                    // mesh_vert.u = uv[i].x;
                    // mesh_vert.v = uv[i].y;

                    // mesh_vert.tx = tangent.x;
                    // mesh_vert.ty = tangent.y;
                    // mesh_vert.tz = tangent.z;

                    mesh_vertices.push_back(mesh_vert);
                }
            }
        }

        // Write the mesh_vertices data into mesh_data
        // Resize the array
        uint32_t stride           = sizeof(MeshVertexDataDefinition);
        mesh_data.m_vertex_buffer = std::make_shared<BufferData>(mesh_vertices.size() * stride); ///< all vertex size
        mesh_data.m_index_buffer  = std::make_shared<BufferData>(
            mesh_vertices.size() * sizeof(uint32_t)); ///< The number of indices is the same as the number of vertices

        assert(mesh_vertices.size() <=
               std::numeric_limits<uint32_t>::max()); ///< Check if the data size exceeds the limit

        uint32_t* indices = (uint32_t*)mesh_data.m_index_buffer->m_data;
        for (size_t i = 0; i < mesh_vertices.size(); i++) ///< write vertex and indice
        {
            ((MeshVertexDataDefinition*)(mesh_data.m_vertex_buffer->m_data))[i] = mesh_vertices[i];
            indices[i] = static_cast<uint32_t>(i); ///< Equal to vertex index
        }

        return mesh_data;
    }
} // namespace VKernel

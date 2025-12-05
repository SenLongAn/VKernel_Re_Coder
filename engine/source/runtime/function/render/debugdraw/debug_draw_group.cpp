#include "runtime/function/render/debugdraw/debug_draw_group.h"
#include "debug_draw_group.h"

namespace VKernel
{
    void DebugDrawGroup::clear()
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        clearData();
    }

    void DebugDrawGroup::clearData()
    {
        m_triangles.clear();
        m_quads.clear();
    }

    void DebugDrawGroup::addTriangle(const Vector3 &point0,
                                     const Vector3 &point1,
                                     const Vector3 &point2,
                                     const Vector4 &color0,
                                     const Vector4 &color1,
                                     const Vector4 &color2,
                                     const Transform &model,
                                     const uint8_t (&indices)[3])
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        DebugDrawTriangle triangle;

        triangle.m_vertex[0].pos = point0;
        triangle.m_vertex[0].color = color0;

        triangle.m_vertex[1].pos = point1;
        triangle.m_vertex[1].color = color1;

        triangle.m_vertex[2].pos = point2;
        triangle.m_vertex[2].color = color2;

        triangle.m_model = model;

        std::memcpy(triangle.m_indices, indices, sizeof(indices));

        m_triangles.push_back(triangle);
    }

    void DebugDrawGroup::addQuad(const Vector3 &point0,
                                 const Vector3 &point1,
                                 const Vector3 &point2,
                                 const Vector3 &point3,
                                 const Vector4 &color0,
                                 const Vector4 &color1,
                                 const Vector4 &color2,
                                 const Vector4 &color3,
                                 const Transform &model,
                                 const uint8_t (&indices)[6])
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        DebugDrawQuad quad;

        quad.m_vertex[0].pos = point0;
        quad.m_vertex[0].color = color0;

        quad.m_vertex[1].pos = point1;
        quad.m_vertex[1].color = color1;

        quad.m_vertex[2].pos = point2;
        quad.m_vertex[2].color = color2;

        quad.m_vertex[3].pos = point3;
        quad.m_vertex[3].color = color3;

        quad.m_model = model;

        std::memcpy(quad.m_indices, indices, sizeof(indices));

        m_quads.push_back(quad);
    }

    size_t DebugDrawGroup::getTriangleCount() const
    {
        // The list needs to be traversed in order to obtain the number of nodes.
        size_t triangle_count = 0;
        for (const DebugDrawTriangle triangle : m_triangles)
        {
            triangle_count++;
        }
        return triangle_count;
    }

    size_t DebugDrawGroup::getQuadCount() const
    {
        size_t quad_count = 0;
        for (const DebugDrawQuad quad : m_quads)
        {
            quad_count++;
        }
        return quad_count;
    }

    void DebugDrawGroup::writeTriangleData(std::vector<DebugDrawVertex> &vertexs)
    {
        // resize
        size_t vertexs_count = getTriangleCount() * 3;
        vertexs.resize(vertexs_count);

        // Traverse and write the data
        size_t current_index = 0;
        for (DebugDrawTriangle triangle : m_triangles)
        {
            vertexs[current_index++] = triangle.m_vertex[0];
            vertexs[current_index++] = triangle.m_vertex[1];
            vertexs[current_index++] = triangle.m_vertex[2];
        }
    }

    void DebugDrawGroup::writeQuadData(std::vector<DebugDrawVertex> &vertexs)
    {
        size_t vertexs_count = getQuadCount() * 4;
        vertexs.resize(vertexs_count);

        size_t current_index = 0;
        for (DebugDrawQuad quad : m_quads)
        {
            vertexs[current_index++] = quad.m_vertex[0];
            vertexs[current_index++] = quad.m_vertex[1];
            vertexs[current_index++] = quad.m_vertex[2];
            vertexs[current_index++] = quad.m_vertex[3];
        }
    }

    void DebugDrawGroup::writeTriangleIndiceData(std::vector<uint8_t> &indices)
    {
        size_t indices_count = getTriangleCount() * 3;
        indices.resize(indices_count);

        size_t current_index = 0;
        for (DebugDrawTriangle triangle : m_triangles)
        {
            indices[current_index++] = triangle.m_indices[0];
            indices[current_index++] = triangle.m_indices[1];
            indices[current_index++] = triangle.m_indices[2];
        }
    }

    void DebugDrawGroup::writeQuadIndiceData(std::vector<uint8_t> &indices)
    {
        size_t indices_count = getQuadCount() * 6;
        indices.resize(indices_count);

        size_t current_index = 0;
        for (DebugDrawQuad quad : m_quads)
        {
            indices[current_index++] = quad.m_indices[0];
            indices[current_index++] = quad.m_indices[1];
            indices[current_index++] = quad.m_indices[2];
            indices[current_index++] = quad.m_indices[3];
            indices[current_index++] = quad.m_indices[4];
            indices[current_index++] = quad.m_indices[5];
        }
    }

    void DebugDrawGroup::writeUniformDynamicDataToCache(std::vector<Matrix4x4> &datas)
    {
        // triangle
        size_t triangle_count = getTriangleCount();
        datas.resize(triangle_count);

        size_t current_index = 0;
        for (DebugDrawTriangle triangle : m_triangles)
        {
            datas[current_index++] = triangle.m_model.getMatrix();
        }

        // quad
        size_t quad_count = getQuadCount();
        current_index = datas.size(); // last offset
        datas.resize(datas.size() + quad_count);

        for (DebugDrawQuad quad : m_quads)
        {
            datas[current_index++] = quad.m_model.getMatrix();
        }
    }
}
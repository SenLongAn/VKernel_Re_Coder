#include "runtime/function/render/debugdraw/debug_draw_group.h"

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
    }

    void DebugDrawGroup::addTriangle(const Vector3 &point0,
                                     const Vector3 &point1,
                                     const Vector3 &point2,
                                     const Vector4 &color0,
                                     const Vector4 &color1,
                                     const Vector4 &color2,
                                     const Transform &model)
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

        m_triangles.push_back(triangle);
    }

    size_t DebugDrawGroup::getTriangleCount() const
    {
        size_t triangle_count = 0;
        for (const DebugDrawTriangle triangle : m_triangles)
        {
            triangle_count++;
        }
        return triangle_count;
    }

    void DebugDrawGroup::writeTriangleData(std::vector<DebugDrawVertex> &vertexs)
    {
        size_t vertexs_count = getTriangleCount() * 3;
        vertexs.resize(vertexs_count);

        size_t current_index = 0;
        for (DebugDrawTriangle triangle : m_triangles)
        {
            vertexs[current_index++] = triangle.m_vertex[0];
            vertexs[current_index++] = triangle.m_vertex[1];
            vertexs[current_index++] = triangle.m_vertex[2];
        }
    }

    void DebugDrawGroup::writeUniformDataToCache(std::vector<std::pair<Matrix4x4, Matrix4x4>> &datas)
    {
        size_t triangle_count = getTriangleCount();
        datas.resize(triangle_count);

        size_t current_index = 0;
        for (DebugDrawTriangle triangle : m_triangles)
        {
            datas[current_index++].first = triangle.m_model.getMatrix();
        }
    }
}
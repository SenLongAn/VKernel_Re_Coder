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
        m_boxes.clear();
        m_spheres.clear();
    }

    void DebugDrawGroup::addTriangle(const Vector4 &color,
                                     const Transform &model,
                                     const PrimitiveType &primitive_type,
                                     const bool &is_depth_test)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        DebugDrawTriangle triangle;

        triangle.m_color = color,

        triangle.m_model = model;

        triangle.m_primitive_type = primitive_type;

        triangle.m_is_depth_test = is_depth_test;

        m_triangles.push_back(triangle);
    }

    void DebugDrawGroup::addQuad(const Vector4 &color,
                                 const Transform &model,
                                 const PrimitiveType &primitive_type,
                                 const bool &is_depth_test)

    {
        std::lock_guard<std::mutex> guard(m_mutex);

        DebugDrawQuad quad;

        quad.m_color = color,

        quad.m_model = model;

        quad.m_primitive_type = primitive_type;

        quad.m_is_depth_test = is_depth_test;

        m_quads.push_back(quad);
    }

    void DebugDrawGroup::addBox(const Vector4 &color,
                                const Transform &model,
                                const PrimitiveType &primitive_type,
                                const bool &is_depth_test)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        DebugDrawBox box;

        box.m_color = color,

        box.m_model = model;

        box.m_primitive_type = primitive_type;

        box.m_is_depth_test = is_depth_test;

        m_boxes.push_back(box);
    }

    void DebugDrawGroup::addSphere(const Vector4 &color,
                                   const Transform &model,
                                   const PrimitiveType &primitive_type,
                                   const bool &is_depth_test)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        DebugDrawSphere sphere;

        sphere.m_color = color,

        sphere.m_model = model;

        sphere.m_primitive_type = primitive_type;

        sphere.m_is_depth_test = is_depth_test;

        m_spheres.push_back(sphere);
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

    size_t DebugDrawGroup::getBoxCount() const
    {
        size_t box_count = 0;
        for (const DebugDrawBox box : m_boxes)
        {
            box_count++;
        }
        return box_count;
    }

    size_t DebugDrawGroup::getSphereCount() const
    {
        size_t sphere_count = 0;
        for (const DebugDrawSphere sphere : m_spheres)
        {
            sphere_count++;
        }
        return sphere_count;
    }

    void DebugDrawGroup::writeVertexData(std::vector<DebugDrawVertex> &vertexs)
    {
        // triangle, quad, box, sphere
        vertexs.resize(
            3 + 4 + 8 + 4225 +  // point
            6 + 8 + 24 + 1720 + // line
            3 + 4 + 8 + 4225    // triangle
        );

        int index = 0;

        // ==================== point ====================

        // triangle_point
        vertexs[index++].pos = Vector3(0.0, -0.5, 0.0);
        vertexs[index++].pos = Vector3(0.5, 0.5, 0.0);
        vertexs[index++].pos = Vector3(-0.5, 0.5, 0.0);

        // quad_point
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, 0.0f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, 0.0f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, 0.0f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, 0.0f);

        // box_point
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, -0.5f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, -0.5f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, -0.5f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, -0.5f);
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, 0.5f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, 0.5f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, 0.5f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, 0.5f);

        // sphere_point
        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        for (int y = 0; y <= Y_SEGMENTS; y++)
        {
            for (int x = 0; x <= X_SEGMENTS; x++)
            {
                float xSegment = (float)x / (float)X_SEGMENTS; ///< 0 -- 1
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = Math::cos(xSegment * 2.0f * Math_PI) * Math::sin(ySegment * Math_PI); ///< radians
                float yPos = Math::cos(ySegment * Math_PI);
                float zPos = Math::sin(xSegment * 2.0f * Math_PI) * Math::sin(ySegment * Math_PI);

                vertexs[index++].pos = Vector3(xPos, yPos, zPos);
            }
        }

        // ==================== line ====================

        // triangle_line
        vertexs[index++].pos = Vector3(-0.5, 0.5, 0.0);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, 0.0f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, 0.0f);
        vertexs[index++].pos = Vector3(0.0f, -0.5f, 0.0f);
        vertexs[index++].pos = Vector3(0.0f, -0.5f, 0.0f);
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, 0.0f);

        // quad_line
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, 0.0f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, 0.0f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, 0.0f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, 0.0f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, 0.0f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, 0.0f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, 0.0f);
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, 0.0f);

        // box_line
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, -0.5f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, -0.5f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, -0.5f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, -0.5f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, -0.5f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, -0.5f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, -0.5f);
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, -0.5f);
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, 0.5f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, 0.5f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, 0.5f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, 0.5f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, 0.5f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, 0.5f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, 0.5f);
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, 0.5f);
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, -0.5f);
        vertexs[index++].pos = Vector3(-0.5f, 0.5f, 0.5f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, -0.5f);
        vertexs[index++].pos = Vector3(0.5f, 0.5f, 0.5f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, -0.5f);
        vertexs[index++].pos = Vector3(0.5f, -0.5f, 0.5f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, -0.5f);
        vertexs[index++].pos = Vector3(-0.5f, -0.5f, 0.5f);

        // sphere_line
        int32_t param = 10;
        float _2pi = 2.0f * Math_PI;
        for (int32_t i = -param - 1; i < param + 1; i++)
        {
            float h = Math::sin(_2pi / 4.0f * i / (param + 1.0f));
            float h1 = Math::sin(_2pi / 4.0f * (i + 1) / (param + 1.0f));
            float r = Math::sqrt(1.0f - h * h);
            float r1 = Math::sqrt(1.0f - h1 * h1);
            for (int32_t j = 0; j < 2 * param; j++)
            {
                Vector3 p(Math::cos(_2pi / (2.0f * param) * j) * r, Math::sin(_2pi / (2.0f * param) * j) * r, h);
                Vector3 p1(Math::cos(_2pi / (2.0f * param) * j) * r1, Math::sin(_2pi / (2.0f * param) * j) * r1, h1);
                vertexs[index++].pos = p;
                vertexs[index++].pos = p1;
            }
            if (i != -param - 1)
            {
                for (int32_t j = 0; j < 2 * param; j++)
                {
                    Vector3 p(Math::cos(_2pi / (2.0f * param) * j) * r, Math::sin(_2pi / (2.0f * param) * j) * r, h);
                    Vector3 p1(Math::cos(_2pi / (2.0f * param) * (j + 1)) * r, Math::sin(_2pi / (2.0f * param) * (j + 1)) * r, h);
                    vertexs[index++].pos = p;
                    vertexs[index++].pos = p1;
                }
            }
        }

        // // ==================== triangle ====================
        // triangle_triangle
        vertexs[index++].pos = Vector3(0.0, -0.5, 0.0);
        vertexs[index++].pos = Vector3(0.5, 0.5, 0.0);
        vertexs[index++].pos = Vector3(-0.5, 0.5, 0.0);

        // quad_triangle
        vertexs[index++].pos = Vector3(-0.5, -0.5, 0.0);
        vertexs[index++].pos = Vector3(0.5, -0.5, 0.0);
        vertexs[index++].pos = Vector3(0.5, 0.5, 0.0);
        vertexs[index++].pos = Vector3(-0.5, 0.5, 0.0);

        // box_triangle
        Vector3 half_extends = Vector3(0.5, 0.5, 0.5);
        Vector3 center_point = Vector3(0.0, 0.0, 0.0);
        Vector3 vertices_local[8] = {
            Vector3(-half_extends.x, -half_extends.y, -half_extends.z),
            Vector3(half_extends.x, -half_extends.y, -half_extends.z),
            Vector3(half_extends.x, half_extends.y, -half_extends.z),
            Vector3(-half_extends.x, half_extends.y, -half_extends.z),
            Vector3(-half_extends.x, -half_extends.y, half_extends.z),
            Vector3(half_extends.x, -half_extends.y, half_extends.z),
            Vector3(half_extends.x, half_extends.y, half_extends.z),
            Vector3(-half_extends.x, half_extends.y, half_extends.z)};
        for (size_t i = 0; i < 8; i++)
        {
            vertexs[index++].pos = vertices_local[i] + center_point;
        }

        // sphere_triangle
        for (int y = 0; y <= Y_SEGMENTS; y++)
        {
            for (int x = 0; x <= X_SEGMENTS; x++)
            {
                float xSegment = (float)x / (float)X_SEGMENTS; ///< 0 -- 1
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = Math::cos(xSegment * 2.0f * Math_PI) * Math::sin(ySegment * Math_PI); ///< radians
                float yPos = Math::cos(ySegment * Math_PI);
                float zPos = Math::sin(xSegment * 2.0f * Math_PI) * Math::sin(ySegment * Math_PI);

                vertexs[index++].pos = Vector3(xPos, yPos, zPos);
            }
        }
    }

    void DebugDrawGroup::writeIndiceData(std::vector<uint16_t> &indices)
    {
        // This is only used for triangle primitives, not for point and line primitives.
        indices.resize(3);
        indices.resize(3 + 6 + 36 + 24576);

        int index = 0;

        // triangle
        indices[index++] = 0;
        indices[index++] = 1;
        indices[index++] = 2;

        // quad
        indices[index++] = 0;
        indices[index++] = 1;
        indices[index++] = 2;
        indices[index++] = 2;
        indices[index++] = 3;
        indices[index++] = 0;

        // box
        uint16_t box_indices[36] = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4,
                                    0, 3, 7, 7, 4, 0, 1, 2, 6, 6, 5, 1,
                                    0, 1, 5, 5, 4, 0, 3, 2, 6, 6, 7, 3};
        for (int i = 0; i < 36; i++)
            indices[index++] = box_indices[i];

        // sphere
        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        for (int i = 0; i < Y_SEGMENTS; i++)
        {
            for (int j = 0; j < X_SEGMENTS; j++)
            {

                indices[index++] = (i * (X_SEGMENTS + 1) + j);
                indices[index++] = ((i + 1) * (X_SEGMENTS + 1) + j);
                indices[index++] = ((i + 1) * (X_SEGMENTS + 1) + j + 1);

                indices[index++] = (i * (X_SEGMENTS + 1) + j);
                indices[index++] = ((i + 1) * (X_SEGMENTS + 1) + j + 1);
                indices[index++] = (i * (X_SEGMENTS + 1) + j + 1);
            }
        }
    }

    std::vector<size_t> DebugDrawGroup::writeUniformDynamicDataToCache(std::vector<std::pair<Matrix4x4, Vector4>> &datas, const PrimitiveType &primity_type, const bool &is_depth_test)
    {
        std::vector<size_t> mesh_count;
        size_t last_size = 0;

        // triangle
        size_t triangle_count = getTriangleCount();
        for (DebugDrawTriangle triangle : m_triangles)
        {
            if (triangle.m_primitive_type == primity_type && triangle.m_is_depth_test == is_depth_test)
            {
                datas.push_back(std::make_pair(triangle.m_model.getMatrix(), triangle.m_color));
            }
        }
        mesh_count.push_back(datas.size() - last_size);
        last_size = datas.size();

        // quad
        size_t quad_count = getQuadCount();
        for (DebugDrawQuad quad : m_quads)
        {
            if (quad.m_primitive_type == primity_type && quad.m_is_depth_test == is_depth_test)
            {
                datas.push_back(std::make_pair(quad.m_model.getMatrix(), quad.m_color));
            }
        }
        mesh_count.push_back(datas.size() - last_size);
        last_size = datas.size();

        // box
        size_t box_count = getBoxCount();
        for (DebugDrawBox box : m_boxes)
        {
            if (box.m_primitive_type == primity_type && box.m_is_depth_test == is_depth_test)
            {
                datas.push_back(std::make_pair(box.m_model.getMatrix(), box.m_color));
            }
        }
        mesh_count.push_back(datas.size() - last_size);
        last_size = datas.size();

        // sphere
        size_t sphere_count = getSphereCount();
        for (DebugDrawSphere sphere : m_spheres)
        {
            if (sphere.m_primitive_type == primity_type && sphere.m_is_depth_test == is_depth_test)
            {
                datas.push_back(std::make_pair(sphere.m_model.getMatrix(), sphere.m_color));
            }
        }
        mesh_count.push_back(datas.size() - last_size);
        last_size = datas.size();

        return mesh_count;
    }
}
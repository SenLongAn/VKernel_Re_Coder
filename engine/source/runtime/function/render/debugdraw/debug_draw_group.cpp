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

    void DebugDrawGroup::addCylinder(const Vector4 &color, const Transform &model, const PrimitiveType &primitive_type, const bool &is_depth_test)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        DebugDrawCylinder cylinder;

        cylinder.m_color = color,

        cylinder.m_model = model;

        cylinder.m_primitive_type = primitive_type;

        cylinder.m_is_depth_test = is_depth_test;

        m_cylinders.push_back(cylinder);
    }

    void DebugDrawGroup::addCapsule(const Vector4 &color, const Transform &model, const PrimitiveType &primitive_type, const bool &is_depth_test)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        DebugDrawCapsule capsule;

        capsule.m_color = color,

        capsule.m_model = model;

        capsule.m_primitive_type = primitive_type;

        capsule.m_is_depth_test = is_depth_test;

        m_capsules.push_back(capsule);
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

    size_t DebugDrawGroup::getCylinderCount() const
    {
        size_t cylinder_count = 0;
        for (const DebugDrawCylinder cylinder : m_cylinders)
        {
            cylinder_count++;
        }
        return cylinder_count;
    }

    size_t DebugDrawGroup::getCapsuleCount() const
    {
        size_t capsule_count = 0;
        for (const DebugDrawCapsule capsule : m_capsules)
        {
            capsule_count++;
        }
        return capsule_count;
    }

    void DebugDrawGroup::writeVertexData(std::vector<DebugDrawVertex> &vertexs)
    {
        // triangle, quad, box, sphere
        vertexs.resize(
            3 + 4 + 8 + 4225 + 44 + 506 +    // point
            6 + 8 + 24 + 1720 + 200 + 1640 + // line
            3 + 4 + 8 + 4225 + 44 + 506      // triangle
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

        // cylinder_point
        int32_t param = 10;
        float _2pi = 2.0f * Math_PI;
        for (int32_t i = 0; i <= 2 * param; i++)
        {
            Vector3 top_point(Math::cos(_2pi / (2.0f * param) * i),
                              Math::sin(_2pi / (2.0f * param) * i),
                              1.0f);

            Vector3 bottom_point(Math::cos(_2pi / (2.0f * param) * i),
                                 Math::sin(_2pi / (2.0f * param) * i),
                                 -1.0f);

            vertexs[index++].pos = top_point;
            vertexs[index++].pos = bottom_point;
        }

        vertexs[index++].pos = Vector3(0.0f, 0.0f, 1.0f);
        vertexs[index++].pos = Vector3(0.0f, 0.0f, -1.0f);

        // capluse_point
        for (int32_t i = 0; i <= param; i++)
        {
            float h = Math::sin(_2pi / 4.0f / param * i);
            float r = Math::sqrt(1 - h * h);

            for (int32_t j = 0; j <= 2 * param; j++)
            {
                Vector3 p(Math::cos(_2pi / (2.0f * param) * j) * r,
                          Math::sin(_2pi / (2.0f * param) * j) * r,
                          h + 1.0f);
                vertexs[index++].pos = p;
            }
        }

        for (int32_t j = 0; j <= 2 * param; j++)
        {
            Vector3 p(Math::cos(_2pi / (2.0f * param) * j),
                      Math::sin(_2pi / (2.0f * param) * j),
                      1.0f);
            vertexs[index++].pos = p;

            Vector3 p1(Math::cos(_2pi / (2.0f * param) * j),
                       Math::sin(_2pi / (2.0f * param) * j),
                       -1.0f);
            vertexs[index++].pos = p1;
        }

        for (int32_t i = 0; i >= -param; i--)
        {
            float h = Math::sin(_2pi / 4.0f / param * i);
            float r = Math::sqrt(1 - h * h);

            for (int32_t j = 0; j <= 2 * param; j++)
            {
                Vector3 p(Math::cos(_2pi / (2.0f * param) * j) * r,
                          Math::sin(_2pi / (2.0f * param) * j) * r,
                          h - 1.0f);
                vertexs[index++].pos = p;
            }
        }

        vertexs[index++].pos = Vector3(0.0f, 0.0f, 2.0f);
        vertexs[index++].pos = Vector3(0.0f, 0.0f, -2.0f);

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

        // cylinder_line
        for (int32_t i = 0; i < 2 * param; i++)
        {
            Vector3 p(Math::cos(_2pi / (2.0f * param) * i), Math::sin(_2pi / (2.0f * param) * i), 1.0f);
            Vector3 p_(Math::cos(_2pi / (2.0f * param) * (i + 1)), Math::sin(_2pi / (2.0f * param) * (i + 1)), 1.0f);
            Vector3 p1(Math::cos(_2pi / (2.0f * param) * i), Math::sin(_2pi / (2.0f * param) * i), -1.0f);
            Vector3 p1_(Math::cos(_2pi / (2.0f * param) * (i + 1)), Math::sin(_2pi / (2.0f * param) * (i + 1)), -1.0f);

            vertexs[index++].pos = p;
            vertexs[index++].pos = p_;

            vertexs[index++].pos = p1;
            vertexs[index++].pos = p1_;

            vertexs[index++].pos = p;
            vertexs[index++].pos = p1;

            vertexs[index++].pos = p;
            vertexs[index++].pos = Vector3(0.0f, 0.0f, 1.0f);

            vertexs[index++].pos = p1;
            vertexs[index++].pos = Vector3(0.0f, 0.0f, -1.0f);
        }

        // capsule_line
        for (int32_t i = 0; i < param; i++)
        {
            float h = Math::sin(_2pi / 4.0 / param * i);
            float h1 = Math::sin(_2pi / 4.0 / param * (i + 1));
            float r = Math::sqrt(1 - h * h);
            float r1 = Math::sqrt(1 - h1 * h1);
            for (int32_t j = 0; j < 2 * param; j++)
            {
                Vector3 p(Math::cos(_2pi / (2.0f * param) * j) * r, Math::sin(_2pi / (2.0f * param) * j) * r, h + 1.0f);
                Vector3 p_(Math::cos(_2pi / (2.0f * param) * (j + 1)) * r, Math::sin(_2pi / (2.0f * param) * (j + 1)) * r, h + 1.0f);
                Vector3 p1(Math::cos(_2pi / (2.0f * param) * j) * r1, Math::sin(_2pi / (2.0f * param) * j) * r1, h1 + 1.0f);
                vertexs[index++].pos = p;
                vertexs[index++].pos = p1;

                vertexs[index++].pos = p;
                vertexs[index++].pos = p_;
            }
        }
        for (int32_t j = 0; j < 2 * param; j++)
        {
            Vector3 p(Math::cos(_2pi / (2.0f * param) * j), Math::sin(_2pi / (2.0f * param) * j), 1.0f);
            Vector3 p1(Math::cos(_2pi / (2.0f * param) * j), Math::sin(_2pi / (2.0f * param) * j), -1.0f);
            vertexs[index++].pos = p;
            vertexs[index++].pos = p1;
        }
        for (int32_t i = 0; i > -param; i--)
        {
            float h = Math::sin(_2pi / 4.0f / param * i);
            float h1 = Math::sin(_2pi / 4.0f / param * (i - 1));
            float r = Math::sqrt(1 - h * h);
            float r1 = Math::sqrt(1 - h1 * h1);
            for (int32_t j = 0; j < (2 * param); j++)
            {
                Vector3 p(Math::cos(_2pi / (2.0f * param) * j) * r, Math::sin(_2pi / (2.0f * param) * j) * r, h - 1.0f);
                Vector3 p_(Math::cos(_2pi / (2.0f * param) * (j + 1)) * r, Math::sin(_2pi / (2.0f * param) * (j + 1)) * r, h - 1.0f);
                Vector3 p1(Math::cos(_2pi / (2.0f * param) * j) * r1, Math::sin(_2pi / (2.0f * param) * j) * r1, h1 - 1.0f);
                vertexs[index++].pos = p;
                vertexs[index++].pos = p1;

                vertexs[index++].pos = p;
                vertexs[index++].pos = p_;
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

        // cylinder_triangle
        for (int32_t i = 0; i <= 2 * param; i++)
        {
            Vector3 top_point(Math::cos(_2pi / (2.0f * param) * i),
                              Math::sin(_2pi / (2.0f * param) * i),
                              1.0f);

            Vector3 bottom_point(Math::cos(_2pi / (2.0f * param) * i),
                                 Math::sin(_2pi / (2.0f * param) * i),
                                 -1.0f);

            vertexs[index++].pos = top_point;
            vertexs[index++].pos = bottom_point;
        }
        top_center_idx = index;
        vertexs[index++].pos = Vector3(0.0f, 0.0f, 1.0f);
        bottom_center_idx = index;
        vertexs[index++].pos = Vector3(0.0f, 0.0f, -1.0f);

        // capluse_triangle
        for (int32_t i = 0; i <= param; i++)
        {
            float h = Math::sin(_2pi / 4.0f / param * i);
            float r = Math::sqrt(1 - h * h);

            for (int32_t j = 0; j <= 2 * param; j++)
            {
                Vector3 p(Math::cos(_2pi / (2.0f * param) * j) * r,
                          Math::sin(_2pi / (2.0f * param) * j) * r,
                          h + 1.0f);
                vertexs[index++].pos = p;
            }
        }

        for (int32_t j = 0; j <= 2 * param; j++)
        {
            Vector3 p(Math::cos(_2pi / (2.0f * param) * j),
                      Math::sin(_2pi / (2.0f * param) * j),
                      1.0f);
            vertexs[index++].pos = p;

            Vector3 p1(Math::cos(_2pi / (2.0f * param) * j),
                       Math::sin(_2pi / (2.0f * param) * j),
                       -1.0f);
            vertexs[index++].pos = p1;
        }

        for (int32_t i = 0; i >= -param; i--)
        {
            float h = Math::sin(_2pi / 4.0f / param * i);
            float r = Math::sqrt(1 - h * h);

            for (int32_t j = 0; j <= 2 * param; j++)
            {
                Vector3 p(Math::cos(_2pi / (2.0f * param) * j) * r,
                          Math::sin(_2pi / (2.0f * param) * j) * r,
                          h - 1.0f);
                vertexs[index++].pos = p;
            }
        }

        vertexs[index++].pos = Vector3(0.0f, 0.0f, 2.0f);
        vertexs[index++].pos = Vector3(0.0f, 0.0f, -2.0f);
    }

    void DebugDrawGroup::writeIndiceData(std::vector<uint16_t> &indices)
    {
        // This is only used for triangle primitives, not for point and line primitives.
        indices.resize(3 + 6 + 36 + 24576 + 240 + 2640);

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

        // cylinder
        for (int32_t i = 0; i < 20; i++)
        {
            int32_t t0 = 2 * i;
            int32_t t1 = 2 * (i + 1);
            int32_t b0 = 2 * i + 1;
            int32_t b1 = 2 * (i + 1) + 1;

            indices[index++] = t0;
            indices[index++] = b0;
            indices[index++] = t1;

            indices[index++] = b0;
            indices[index++] = b1;
            indices[index++] = t1;
        }

        for (int32_t i = 0; i < 20; i++)
        {
            indices[index++] = top_center_idx;
            indices[index++] = 2 * i;
            indices[index++] = 2 * (i + 1);
        }

        for (int32_t i = 0; i < 20; i++)
        {
            indices[index++] = bottom_center_idx;
            indices[index++] = 2 * (i + 1) + 1;
            indices[index++] = 2 * i + 1;
        }

        // capluse
        int32_t vert_per_layer = 2 * 10 + 1;
        int32_t top_sphere_layers = 10 + 1;
        int32_t cylinder_top_start = top_sphere_layers * vert_per_layer;
        int32_t cylinder_bottom_start = cylinder_top_start + vert_per_layer;
        int32_t bottom_sphere_start = cylinder_bottom_start + vert_per_layer;
        int32_t top_pole = 506 - 2;
        int32_t bottom_pole = 506 - 1;

        for (int32_t i = 0; i < top_sphere_layers - 1; i++)
        {
            for (int32_t j = 0; j < 2 * 10; j++)
            {
                int32_t current = i * vert_per_layer + j;
                int32_t next = i * vert_per_layer + j + 1;
                int32_t below = (i + 1) * vert_per_layer + j;
                int32_t below_next = (i + 1) * vert_per_layer + j + 1;

                indices[index++] = current;
                indices[index++] = below;
                indices[index++] = next;

                indices[index++] = below;
                indices[index++] = below_next;
                indices[index++] = next;
            }
        }

        for (int32_t i = 0; i < 2 * 10; i++)
        {
            int32_t top_current = cylinder_top_start + i;
            int32_t top_next = cylinder_top_start + i + 1;
            int32_t bottom_current = cylinder_bottom_start + i;
            int32_t bottom_next = cylinder_bottom_start + i + 1;

            indices[index++] = top_current;
            indices[index++] = bottom_current;
            indices[index++] = top_next;

            indices[index++] = bottom_current;
            indices[index++] = bottom_next;
            indices[index++] = top_next;
        }

        int32_t bottom_sphere_layers = 10 + 1;
        int32_t bottom_sphere_base = bottom_sphere_start;

        for (int32_t i = 0; i < bottom_sphere_layers - 1; i++)
        {
            for (int32_t j = 0; j < 2 * 10; j++)
            {
                int32_t current = bottom_sphere_base + i * vert_per_layer + j;
                int32_t next = bottom_sphere_base + i * vert_per_layer + j + 1;
                int32_t below = bottom_sphere_base + (i + 1) * vert_per_layer + j;
                int32_t below_next = bottom_sphere_base + (i + 1) * vert_per_layer + j + 1;

                indices[index++] = current;
                indices[index++] = below;
                indices[index++] = next;

                indices[index++] = below;
                indices[index++] = below_next;
                indices[index++] = next;
            }
        }

        for (int32_t j = 0; j < 2 * 10; j++)
        {
            int32_t top_current = vert_per_layer * (top_sphere_layers - 1) + j;
            int32_t top_next = vert_per_layer * (top_sphere_layers - 1) + j + 1;

            indices[index++] = top_pole;
            indices[index++] = top_next;
            indices[index++] = top_current;

            int32_t bottom_current = bottom_sphere_base + vert_per_layer * (bottom_sphere_layers - 1) + j;
            int32_t bottom_next = bottom_sphere_base + vert_per_layer * (bottom_sphere_layers - 1) + j + 1;

            indices[index++] = bottom_pole;
            indices[index++] = bottom_current;
            indices[index++] = bottom_next;
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

        // cylinder
        size_t cylinder_count = getCylinderCount();
        for (DebugDrawCylinder cylinder : m_cylinders)
        {
            if (cylinder.m_primitive_type == primity_type && cylinder.m_is_depth_test == is_depth_test)
            {
                datas.push_back(std::make_pair(cylinder.m_model.getMatrix(), cylinder.m_color));
            }
        }
        mesh_count.push_back(datas.size() - last_size);
        last_size = datas.size();

        // capsule
        size_t capsule_count = getCapsuleCount();
        for (DebugDrawCapsule capsule : m_capsules)
        {
            if (capsule.m_primitive_type == primity_type && capsule.m_is_depth_test == is_depth_test)
            {
                datas.push_back(std::make_pair(capsule.m_model.getMatrix(), capsule.m_color));
            }
        }
        mesh_count.push_back(datas.size() - last_size);
        last_size = datas.size();

        return mesh_count;
    }
}
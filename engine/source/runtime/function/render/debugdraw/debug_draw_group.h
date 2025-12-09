#pragma once

#include "runtime/function/render/debugdraw/debug_draw_primitive.h"

#include <mutex>
#include <list>

/**
 * primitive group
 */
namespace VKernel
{
    class DebugDrawGroup ///< primitive group
    {

    public:
        ~DebugDrawGroup() { clear(); } ///< Destructor
        void initialize() {}           ///< init
        void clear();                  ///< clear
        void clearData();              ///< clear

        // add
        void addTriangle(const Vector4 &color,
                         const Transform &model,
                         const PrimitiveType &primitive_type,
                         const bool &is_depth_test);

        void addQuad(const Vector4 &color,
                     const Transform &model,
                     const PrimitiveType &primitive_type,
                     const bool &is_depth_test);

        void addBox(const Vector4 &color,
                    const Transform &model,
                    const PrimitiveType &primitive_type,
                    const bool &is_depth_test);

        void addSphere(const Vector4 &color,
                       const Transform &model,
                       const PrimitiveType &primitive_type,
                       const bool &is_depth_test);

        void addCylinder(const Vector4 &color,
                         const Transform &model,
                         const PrimitiveType &primitive_type,
                         const bool &is_depth_test);

        void addCapsule(const Vector4 &color,
                         const Transform &model,
                         const PrimitiveType &primitive_type,
                         const bool &is_depth_test);

        // get
        size_t getTriangleCount() const;
        size_t getQuadCount() const;
        size_t getBoxCount() const;
        size_t getSphereCount() const;
        size_t getCylinderCount() const;
        size_t getCapsuleCount() const;

        // The input parameter that writes the vertex data
        void writeVertexData(std::vector<DebugDrawVertex> &vertexs);

        // The input parameter that writes the indice data
        void writeIndiceData(std::vector<uint16_t> &indices);

        // The input parameter that writes the udbo data
        std::vector<size_t> writeUniformDynamicDataToCache(std::vector<std::pair<Matrix4x4, Vector4>> &datas, const PrimitiveType &primity_type, const bool &is_depth_test);

    private:
        std::mutex m_mutex; ///< shared data

        // primitive group
        std::list<DebugDrawTriangle> m_triangles;
        std::list<DebugDrawQuad> m_quads;
        std::list<DebugDrawBox> m_boxes;
        std::list<DebugDrawSphere> m_spheres;
        std::list<DebugDrawCylinder> m_cylinders;
        std::list<DebugDrawCapsule> m_capsules;

        int32_t top_center_idx = 0;
        int32_t bottom_center_idx = 0;
    };
}
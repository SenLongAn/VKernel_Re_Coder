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
        void addTriangle(const Vector3 &point0,
                         const Vector3 &point1,
                         const Vector3 &point2,
                         const Vector4 &color0,
                         const Vector4 &color1,
                         const Vector4 &color2,
                         const Transform &model,
                         const uint16_t (&indices)[3]);

        void addQuad(const Vector3 &point0,
                     const Vector3 &point1,
                     const Vector3 &point2,
                     const Vector3 &point3,
                     const Vector4 &color0,
                     const Vector4 &color1,
                     const Vector4 &color2,
                     const Vector4 &color3,
                     const Transform &model,
                     const uint16_t (&indices)[6]);

        void addBox(const Vector3 &center_point,
                    const Vector3 &half_extends,
                    const Vector4 &color0,
                    const Vector4 &color1,
                    const Vector4 &color2,
                    const Vector4 &color3,
                    const Vector4 &color4,
                    const Vector4 &color5,
                    const Vector4 &color6,
                    const Vector4 &color7,
                    const Transform &model,
                    const uint16_t (&indices)[36]);

        void addSphere(const Vector4 &color,
                       const Transform &model);

        // get
        size_t getTriangleCount() const;
        size_t getQuadCount() const;
        size_t getBoxCount() const;
        size_t getSphereCount() const;

        // The input parameter that writes the vertex data
        void writeTriangleData(std::vector<DebugDrawVertex> &vertexs);
        void writeQuadData(std::vector<DebugDrawVertex> &vertexs);
        void writeBoxData(std::vector<DebugDrawVertex> &vertexs);
        void writeSphereData(std::vector<DebugDrawVertex> &vertexs);

        // The input parameter that writes the indice data
        void writeTriangleIndiceData(std::vector<uint16_t> &indices);
        void writeQuadIndiceData(std::vector<uint16_t> &indices);
        void writeBoxIndiceData(std::vector<uint16_t> &indices);

        // The input parameter that writes the vertex data
        void writeUniformDynamicDataToCache(std::vector<Matrix4x4> &datas);

    private:
        std::mutex m_mutex; ///< shared data

        // primitive group
        std::list<DebugDrawTriangle> m_triangles;
        std::list<DebugDrawQuad> m_quads;
        std::list<DebugDrawBox> m_boxes;
        std::list<DebugDrawSphere> m_spheres;
    };
}
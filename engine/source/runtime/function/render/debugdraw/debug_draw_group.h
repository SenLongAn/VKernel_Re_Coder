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
                         const Transform &model);

        void addQuad(const Vector3 &point0,
                     const Vector3 &point1,
                     const Vector3 &point2,
                     const Vector3 &point3,
                     const Vector3 &point4,
                     const Vector3 &point5,
                     const Vector4 &color0,
                     const Vector4 &color1,
                     const Vector4 &color2,
                     const Vector4 &color3,
                     const Vector4 &color4,
                     const Vector4 &color5,
                     const Transform &model);

        // get
        size_t getTriangleCount() const;
        size_t getQuadCount() const;

        // The input parameter that writes the vertex data
        void writeTriangleData(std::vector<DebugDrawVertex> &vertexs);
        void writeQuadData(std::vector<DebugDrawVertex> &vertexs);

        // The input parameter that writes the vertex data
        void writeUniformDataToCache(std::vector<std::pair<Matrix4x4, Matrix4x4>> &datas, const Matrix4x4& viewProjMatrix);

    private:
        std::mutex m_mutex; ///< shared data

        // primitive group
        std::list<DebugDrawTriangle> m_triangles;
        std::list<DebugDrawQuad> m_quads;
    };
}
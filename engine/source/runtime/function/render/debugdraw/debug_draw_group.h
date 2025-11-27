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
        ~DebugDrawGroup(){clear();} ///< Destructor
        void initialize(){} ///< init
        void clear(); ///< clear
        void clearData(); ///< clear

        // add
        void addTriangle(const Vector3& point0,
                         const Vector3& point1,
                         const Vector3& point2,
                         const Vector4& color0,
                         const Vector4& color1,
                         const Vector4& color2,
                         const Transform& model);
        
        // get     
        size_t getTriangleCount() const;
    
        // Write the graphic elements into the vertex data.
        void writeTriangleData(std::vector<DebugDrawVertex>& vertexs);
        void writeUniformDynamicDataToCache(std::vector<Matrix4x4>& datas);

    private:
        std::mutex m_mutex; ///< shared data

        // primitive group
        std::list<DebugDrawTriangle> m_triangles;
    };
}
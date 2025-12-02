#pragma once

#include "runtime/core/math/math_headers.h"

#include <mutex>

/**
 * camera
 */
namespace VKernel
{
    class RenderCamera
    {

    public:
        // get

        Vector3 position() const { return m_position; } ///< position

        Vector3 up() const { return (m_invRotation * Z); }      ///< up vector, The three axes represent rotation.
        Vector3 forward() const { return (m_invRotation * Y); } ///< forward vector

        Matrix4x4 getViewMatrix();           ///< view Matrix
        Matrix4x4 getPersProjMatrix() const; ///< projection Matrix

        // set: Calculate and set the camera properties based on the input values
        void setFOVx(float fovx) { m_fovx = fovx; }
        void setMainViewMatrix(const Matrix4x4 &view_matrix);
        void lookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up);
        void setAspect(float aspect);
        void setZNear(float znear) { m_znear = znear; }
        void setZFar(float zfar) { m_zfar = zfar; }

    protected:
        std::mutex m_view_matrix_mutex; ///< mutex

        // Camera properties
        static const Vector3 X, Y, Z;

        Vector3 m_position{0.0f, 0.0f, 0.0f};           ///< position
        Quaternion m_rotation{Quaternion::IDENTITY};    ///< rotation
        Quaternion m_invRotation{Quaternion::IDENTITY}; ///< invert rotation: Keep the camera position unchanged and apply the inverse transformation of the camera to the object.

        float m_znear{1000.0f}; ///< near plane
        float m_zfar{0.1f};     ///< far plane

        float m_fovx{0.f}; ///< horizontal direction fov
        float m_fovy{0.f}; ///< vertical direction fov

        Matrix4x4 m_view_matrices{Matrix4x4::IDENTITY}; ///< view matrices

        float m_aspect{0.f}; ///< The aspect ratio = viewport width/viewport height
    };

    inline const Vector3 RenderCamera::X = {1.0f, 0.0f, 0.0f};
    inline const Vector3 RenderCamera::Y = {0.0f, 1.0f, 0.0f};
    inline const Vector3 RenderCamera::Z = {0.0f, 0.0f, 1.0f};
}
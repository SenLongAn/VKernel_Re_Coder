#include "runtime/function/render/render_camera.h"
#include "render_camera.h"

#include <iostream>
namespace VKernel
{
    Matrix4x4 RenderCamera::getViewMatrix()
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);


        // Construct a matrix based on the camera attributes
        auto view_matrix = Matrix4x4::IDENTITY;
        view_matrix = Math::makeLookAtMatrix(position(), position() + forward(), up());
        return view_matrix;
    }

    Matrix4x4 RenderCamera::getPersProjMatrix() const
    {
        // Construct a matrix based on the camera attributes
        Matrix4x4 fix_mat(1, 0, 0, 0,
                          0, -1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1);
        Matrix4x4 proj_mat = fix_mat * Math::makePerspectiveMatrix(Radian(Degree(m_fovy)), m_aspect, m_znear, m_zfar);

        return proj_mat;
    }

    void RenderCamera::setMainViewMatrix(const Matrix4x4 &view_matrix) 
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);

        // Extract the position of the camera from the view matrix
        Vector3 s = Vector3(view_matrix[0][0], view_matrix[0][1], view_matrix[0][2]);
        Vector3 u = Vector3(view_matrix[1][0], view_matrix[1][1], view_matrix[1][2]);
        Vector3 f = Vector3(-view_matrix[2][0], -view_matrix[2][1], -view_matrix[2][2]);
        m_position = s * (-view_matrix[0][3]) + u * (-view_matrix[1][3]) + f * view_matrix[2][3];
    }

    void RenderCamera::lookAt(const Vector3& position, const Vector3& target, const Vector3& up)
    {
        m_position = position;

        Vector3 forward = (target - position).normalisedCopy();
        Vector3 upNorm = up.normalisedCopy();
        m_rotation = forward.getRotationTo(Y);
        Vector3 right  = forward.crossProduct(up.normalisedCopy()).normalisedCopy();
        Vector3 orthUp = right.crossProduct(forward);
        Quaternion upRotation = (m_rotation * orthUp).getRotationTo(Z);
        m_rotation = Quaternion(upRotation) * m_rotation;

        m_invRotation = m_rotation.conjugate();
    }

    void RenderCamera::setAspect(float aspect)
    {
        m_aspect = aspect;

        m_fovy = Radian(Math::atan(Math::tan(Radian(Degree(m_fovx) * 0.5f)) / m_aspect) * 2.0f).valueDegrees();
    }
}

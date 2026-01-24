#include "runtime/function/render/render_camera.h"

#include "runtime/resource/res_type/data/camera_config.h"

#include <iostream>
namespace VKernel
{
    Matrix4x4 RenderCamera::getViewMatrix()
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);

        // Construct a matrix based on the camera attributes
        auto view_matrix = Matrix4x4::IDENTITY;
        switch (m_current_camera_type)
        {
            case RenderCameraType::Editor:
                view_matrix = Math::makeLookAtMatrix(position(), position() + forward(), up());
                break;
            case RenderCameraType::Motor:
                view_matrix = m_view_matrices[MAIN_VIEW_MATRIX_INDEX];
                break;
            default:
                break;
        }
        return view_matrix;
    }

    Matrix4x4 RenderCamera::getPersProjMatrix() const
    {
        // Construct a matrix based on the camera attributes
        Matrix4x4 fix_mat(1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        Matrix4x4 proj_mat = fix_mat * Math::makePerspectiveMatrix(Radian(Degree(m_fovy)), m_aspect, m_znear, m_zfar);

        return proj_mat;
    }

    void RenderCamera::setCurrentCameraType(RenderCameraType type)
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);
        m_current_camera_type = type;
    }

    void RenderCamera::setMainViewMatrix(const Matrix4x4& view_matrix, RenderCameraType type)
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);

        // set
        m_current_camera_type                   = type;
        m_view_matrices[MAIN_VIEW_MATRIX_INDEX] = view_matrix;

        // Extract the position of the camera from the view matrix
        Vector3 s  = Vector3(view_matrix[0][0], view_matrix[0][1], view_matrix[0][2]);
        Vector3 u  = Vector3(view_matrix[1][0], view_matrix[1][1], view_matrix[1][2]);
        Vector3 f  = Vector3(-view_matrix[2][0], -view_matrix[2][1], -view_matrix[2][2]);
        m_position = s * (-view_matrix[0][3]) + u * (-view_matrix[1][3]) + f * view_matrix[2][3];
    }

    void RenderCamera::lookAt(const Vector3& position, const Vector3& target, const Vector3& up)
    {
        m_position = position;

        Vector3 forward       = (target - position).normalisedCopy();
        Vector3 upNorm        = up.normalisedCopy();
        m_rotation            = forward.getRotationTo(Y);
        Vector3    right      = forward.crossProduct(up.normalisedCopy()).normalisedCopy();
        Vector3    orthUp     = right.crossProduct(forward);
        Quaternion upRotation = (m_rotation * orthUp).getRotationTo(Z);
        m_rotation            = Quaternion(upRotation) * m_rotation;

        m_invRotation = m_rotation.conjugate();
    }

    void RenderCamera::setAspect(float aspect)
    {
        m_aspect = aspect;

        m_fovy = Radian(Math::atan(Math::tan(Radian(Degree(m_fovx) * 0.5f)) / m_aspect) * 2.0f).valueDegrees();
    }

    void RenderCamera::move(Vector3 delta) { m_position += delta; }

    void RenderCamera::rotate(Vector2 delta)
    {
        // to switch from degree to radian
        delta = Vector2(Radian(Degree(delta.x)).valueRadians(), Radian(Degree(delta.y)).valueRadians());

        Quaternion pitch, yaw;
        pitch.fromAngleAxis(Radian(delta.x), X);
        yaw.fromAngleAxis(-Radian(delta.y), Y);

        m_rotation = pitch * m_rotation * yaw;

        m_invRotation = m_rotation.conjugate();
    }

    void RenderCamera::zoom(float offset) { m_fovx = Math::clamp(m_fovx - offset, MIN_FOV, MAX_FOV); }

    void RenderCamera::resetData(const CameraConfig& camera_config)
    {
        const CameraPose& camera_pose = camera_config.m_pose;
        lookAt(camera_pose.m_position, camera_pose.m_target, camera_pose.m_up);
        setZFar(camera_config.m_z_far);
        setZNear(camera_config.m_z_near);
        m_fovx = {Degree(89.f).valueDegrees()};
    }
} // namespace VKernel

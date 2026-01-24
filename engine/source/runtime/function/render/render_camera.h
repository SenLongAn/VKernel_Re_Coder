#pragma once

#include "runtime/core/math/math_headers.h"

#include <mutex>

/**
 * camera
 */
namespace VKernel
{
    enum class RenderCameraType : int ///< camera type: editor or motor
    {
        Editor,
        Motor
    };

    class CameraConfig;
    class RenderCamera
    {

    public:
        // get
        Vector3 position() const { return m_position; } ///< position

        Vector3 up() const { return (m_invRotation * Z); }      ///< up vector, The three axes represent rotation.
        Vector3 forward() const { return (m_invRotation * Y); } ///< forward vector
        Vector3 right() const { return (m_invRotation * X); }

        Matrix4x4 getViewMatrix();           ///< view Matrix
        Matrix4x4 getPersProjMatrix() const; ///< projection Matrix

        Vector2 getFOV() const { return {m_fovx, m_fovy}; } ///< fov
        float   getFovYDeprecated() const { return m_fovy; }

        Quaternion rotation() const { return m_rotation; }

        Matrix4x4 getLookAtMatrix() const { return Math::makeLookAtMatrix(position(), position() + forward(), up()); }

        // set
        void setFOVx(float fovx) { m_fovx = fovx; }
        void setCurrentCameraType(RenderCameraType type);
        void setMainViewMatrix(const Matrix4x4& view_matrix, RenderCameraType type = RenderCameraType::Editor);
        void lookAt(const Vector3& position, const Vector3& target, const Vector3& up);
        void setAspect(float aspect);
        void setZNear(float znear) { m_znear = znear; }
        void setZFar(float zfar) { m_zfar = zfar; }

        // update
        void move(Vector3 delta);
        void rotate(Vector2 delta);
        void zoom(float offset);

        void resetData(const CameraConfig& camera_config);

    protected:
        std::mutex m_view_matrix_mutex; ///< mutex

        RenderCameraType m_current_camera_type {RenderCameraType::Editor}; ///< camera type

        static constexpr int MAIN_VIEW_MATRIX_INDEX {0}; ///< array index

        std::vector<Matrix4x4> m_view_matrices {Matrix4x4::IDENTITY}; ///< VP matrix

        // Camera properties
        static const Vector3 X, Y, Z;

        Vector3    m_position {0.0f, 0.0f, 0.0f};     ///< position
        Quaternion m_rotation {Quaternion::IDENTITY}; ///< rotation
        Quaternion m_invRotation {
            Quaternion::IDENTITY}; ///< invert rotation: Keep the camera position unchanged and apply the inverse
                                   ///< transformation of the camera to the object.

        float m_znear {1000.0f}; ///< near plane
        float m_zfar {0.1f};     ///< far plane

        float                  m_fovx {Degree(89.f).valueDegrees()}; ///< horizontal direction fov
        float                  m_fovy {0.f};                         ///< vertical direction fov
        float                  m_aspect {0.f}; ///< The aspect ratio = viewport width/viewport height
        static constexpr float MIN_FOV {10.0f};
        static constexpr float MAX_FOV {89.0f};
    };

    inline const Vector3 RenderCamera::X = {1.0f, 0.0f, 0.0f};
    inline const Vector3 RenderCamera::Y = {0.0f, 1.0f, 0.0f};
    inline const Vector3 RenderCamera::Z = {0.0f, 0.0f, 1.0f};
} // namespace VKernel
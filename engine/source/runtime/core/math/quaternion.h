#pragma once

#include "runtime/core/math/math_angle.h"

#include <cassert>

/**
 * Quaternion
 */
namespace VKernel
{
    class Matrix3x3;
    class Vector3;

    class Quaternion
    {
    public:
        float w {1.f}, x {0.f}, y {0.f}, z {0.f};

    public:
        // Constructor
        //-----------------------------------------------------------------------
        Quaternion() = default;
        Quaternion(float w_, float x_, float y_, float z_) : w {w_}, x {x_}, y {y_}, z {z_} {}

        explicit Quaternion(const Matrix3x3& rot) { this->fromRotationMatrix(rot); }
        void fromRotationMatrix(const Matrix3x3& rotation); ///< Construct from RotationMatrix

        Quaternion(const Radian& angle, const Vector3& axis) { this->fromAngleAxis(angle, axis); }
        void fromAngleAxis(const Radian& angle, const Vector3& axis); ///< Construct from angle and axis, Rotate around the axis by angle
        static Quaternion getQuaternionFromAngleAxis(const Radian& angle, const Vector3& axis);
        
        Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
        {
            this->fromAxes(xaxis, yaxis, zaxis);
        }
        void fromAxes(const Vector3& x_axis, const Vector3& y_axis, const Vector3& z_axis); ///< Constructs from 3 axis, the axes are assumed to be orthonormal

        void fromDirection(const Vector3& direction, const Vector3& up_direction); ///< Construct from Direction
        static Quaternion getQuaternionFromDirection(const Vector3& direction, const Vector3& up_direction);

        Quaternion(const Vector3& eulerAngle){ this->fromEulerAngle(eulerAngle); }
        
        // get ptr
        //-----------------------------------------------------------------------
        /// Pointer accessor for direct copying
        float* ptr() { return &w; }

        /// Pointer accessor for direct copying
        const float* ptr() const { return &w; }

        // overloaded operator([], ==, !=, +-*/)
        //-----------------------------------------------------------------------
        Quaternion operator+(const Quaternion& rhs) const
        {
            return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Quaternion operator-(const Quaternion& rhs) const
        {
            return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
        }

        Quaternion mul(const Quaternion& rhs) const { return (*this) * rhs; }
        Quaternion operator*(const Quaternion& rhs) const;

        Quaternion operator*(float scalar) const { return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar); }

        Vector3 operator*(const Vector3& rhs) const;

        Quaternion operator/(float scalar) const
        {
            assert(scalar != 0.0f);
            return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
        }

        friend Quaternion operator*(float scalar, const Quaternion& rhs)
        {
            return Quaternion(scalar * rhs.w, scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
        }

        Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }

        bool operator==(const Quaternion& rhs) const
        {
            return (rhs.x == x) && (rhs.y == y) && (rhs.z == z) && (rhs.w == w);
        }

        bool operator!=(const Quaternion& rhs) const
        {
            return (rhs.x != x) || (rhs.y != y) || (rhs.z != z) || (rhs.w != w);
        }

        // util function
        //-----------------------------------------------------------------------
        // Quaternion <-> euler angle
        Vector3 toEulerAngle() const; ///< Quaternion 2 euler angle
        void fromEulerAngle(const Vector3& eulerAngle); ///< euler angle 2 Quaternion

        // Quaternion 2 RotationMatrix
        void toRotationMatrix(Matrix3x3 & rotation) const;
        void toRotationMatrix(Matrix4x4 & rotation) const;

        // The quaternion representing the rotation is
        void toAngleAxis(Radian & angle, Vector3 & axis) const;

        // Gets the 3 orthonormal axes defining the quaternion
        void toAxes(Vector3 & x_axis, Vector3 & y_axis, Vector3 & z_axis) const;

        // return XYZ orthonormal axis
        Vector3 xAxis() const;
        Vector3 yAxis() const;
        Vector3 zAxis() const;

        // Check whether this quaternion contains valid values
        bool isNaN() const { return Math::isNan(x) || Math::isNan(y) || Math::isNan(z) || Math::isNan(w); }

        // Obtain each component
        float getX() const { return x; }
        float getY() const { return y; }
        float getZ() const { return z; }
        float getW() const { return w; }

        // dot
        float dot(const Quaternion& rkQ) const { return w * rkQ.w + x * rkQ.x + y * rkQ.y + z * rkQ.z; }

        // get length
        float length() const { return std::sqrt(w * w + x * x + y * y + z * z); }

        // Normalizes this quaternion, and returns the previous length
        void normalise(void)
        {
            float factor = 1.0f / this->length();
            *this        = *this * factor;
        }

        // inverse, apply to non-zero quaternion
        Quaternion inverse() const
        {
            float norm = w * w + x * x + y * y + z * z;
            if (norm > 0.0)
            {
                float inv_norm = 1.0f / norm;
                return Quaternion(w * inv_norm, -x * inv_norm, -y * inv_norm, -z * inv_norm);
            }
            else
            {
                return ZERO;
            }
        }

        // get roll, pitch, yaw
        Radian getRoll(bool reproject_axis = true) const;
        Radian getPitch(bool reproject_axis = true) const;
        Radian getYaw(bool reproject_axis = true) const;

        // linear interpolation
        static Quaternion sLerp(float t, const Quaternion& kp, const Quaternion& kq, bool shortest_path = false);
        static Quaternion nLerp(float t, const Quaternion& kp, const Quaternion& kq, bool shortest_path = false);

        // conjugate
        Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }

        // special values
        static const Quaternion ZERO;
        static const Quaternion IDENTITY;

        static const float k_epsilon;
    };
}

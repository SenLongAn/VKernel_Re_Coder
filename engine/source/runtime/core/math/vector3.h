#pragma once

#include "runtime/core/math/math.h"

#include <cassert>
#include <cmath>

/**
 * vector3
 */
namespace VKernel
{
    class Vector3
    {
    public:
        float x {0.f};
        float y {0.f};
        float z {0.f};

    public:
        // Constructor
        Vector3() = default;
        Vector3(float x_, float y_, float z_) : x {x_}, y {y_}, z {z_} {}
        explicit Vector3(const float coords[3]) : x {coords[0]}, y {coords[1]}, z {coords[2]} {}

        // get ptr
        // Pointer accessor for direct copying
        float* ptr() { return &x; }
        // Pointer accessor for direct copying
        const float* ptr() const { return &x; }

        // overloaded operator([], ==, !=, +-*/, +=-=*=/=)
        float operator[](size_t i) const
        {
            assert(i < 3);
            return *(&x + i);
        }

        float& operator[](size_t i)
        {
            assert(i < 3);
            return *(&x + i);
        }

        bool operator==(const Vector3& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }

        bool operator!=(const Vector3& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

        Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }

        Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }

        Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }

        Vector3 operator*(const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }

        Vector3 operator/(float scalar) const
        {
            assert(scalar != 0.0);
            return Vector3(x / scalar, y / scalar, z / scalar);
        }

        Vector3 operator/(const Vector3& rhs) const
        {
            assert((rhs.x != 0 && rhs.y != 0 && rhs.z != 0));
            return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
        }

        const Vector3& operator+() const { return *this; }

        Vector3 operator-() const { return Vector3(-x, -y, -z); }

        friend Vector3 operator*(float scalar, const Vector3& rhs)
        {
            return Vector3(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
        }

        friend Vector3 operator/(float scalar, const Vector3& rhs)
        {
            assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0);
            return Vector3(scalar / rhs.x, scalar / rhs.y, scalar / rhs.z);
        }

        friend Vector3 operator+(const Vector3& lhs, float rhs)
        {
            return Vector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
        }

        friend Vector3 operator+(float lhs, const Vector3& rhs)
        {
            return Vector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
        }

        friend Vector3 operator-(const Vector3& lhs, float rhs)
        {
            return Vector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
        }

        friend Vector3 operator-(float lhs, const Vector3& rhs)
        {
            return Vector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
        }

        Vector3& operator+=(const Vector3& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vector3& operator+=(float scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            return *this;
        }

        Vector3& operator-=(const Vector3& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        Vector3& operator-=(float scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            return *this;
        }

        Vector3& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vector3& operator*=(const Vector3& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }

        Vector3& operator/=(float scalar)
        {
            assert(scalar != 0.0);
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        Vector3& operator/=(const Vector3& rhs)
        {
            assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0);
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            return *this;
        }

        // Arithmetic function

        // length
        // Returns the length of the vector.
        float length() const { return std::hypot(x, y, z); }
        // Returns the square of the length(magnitude) of the vector.
        float squaredLength() const { return x * x + y * y + z * z; }
        // Returns the square of the length(magnitude) of the vector.
        float squaredDistance(const Vector3& rhs) const { return (*this - rhs).squaredLength(); }
        // Returns the distance to another vector.
        float distance(const Vector3& rhs) const { return (*this - rhs).length(); }

        // Calculates the dot (scalar) product of this vector with another.
        float dotProduct(const Vector3& vec) const { return x * vec.x + y * vec.y + z * vec.z; }

        // Normalizes the vector.
        void normalise()
        {
            float length = std::hypot(x, y, z);
            if (length == 0.f)
                return;

            float inv_lengh = 1.0f / length;
            x *= inv_lengh;
            y *= inv_lengh;
            z *= inv_lengh;
        }

        // Calculate the three-dimensional cross product of two vectors. The result is a single-precision floating-point number, which is twice the area of the triangle.
        Vector3 crossProduct(const Vector3& rhs) const
        {
            return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
        }

        // Sets this vector's components to the minimum of its own and the ones of the passed in vector.
        void makeFloor(const Vector3& cmp)
        {
            if (cmp.x < x)
                x = cmp.x;
            if (cmp.y < y)
                y = cmp.y;
            if (cmp.z < z)
                z = cmp.z;
        }
        // Sets this vector's components to the maximum of its own and the ones of the passed in vector.
        void makeCeil(const Vector3& cmp)
        {
            if (cmp.x > x)
                x = cmp.x;
            if (cmp.y > y)
                y = cmp.y;
            if (cmp.z > z)
                z = cmp.z;
        }

        // Returns true if this vector is zero length. 
        bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));
        }
        bool isZero() const { return x == 0.f && y == 0.f && z == 0.f; }

        // As normalise, except that this vector is unaffected and the normalised vector is returned as a copy.
        Vector3 normalisedCopy(void) const
        {
            Vector3 ret = *this;
            ret.normalise();
            return ret;
        }

        // Calculates a reflection vector to the plane with the given normal
        Vector3 reflect(const Vector3& normal) const
        {
            return Vector3(*this - (2 * this->dotProduct(normal) * normal));
        }

        // Calculates projection to a plane with the given normal
        Vector3 project(const Vector3& normal) const { return Vector3(*this - (this->dotProduct(normal) * normal)); }

        // absolute value
        Vector3 absoluteCopy() const { return Vector3(fabsf(x), fabsf(y), fabsf(z)); }

        // linear interpolation
        static Vector3 lerp(const Vector3& lhs, const Vector3& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

        // Limit v within the range of min and max.
        static Vector3 clamp(const Vector3& v, const Vector3& min, const Vector3& max)
        {
            return Vector3(
                Math::clamp(v.x, min.x, max.x), Math::clamp(v.y, min.y, max.y), Math::clamp(v.z, min.z, max.z));
        }

        // get Max Element
        static float getMaxElement(const Vector3& v) { return Math::getMaxElement(v.x, v.y, v.z); }
        
        // Check whether this vector contains valid values
        bool isNaN() const { return Math::isNan(x) || Math::isNan(y) || Math::isNan(z); }
        
        // special value
        static const Vector3 ZERO;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
        static const Vector3 NEGATIVE_UNIT_X;
        static const Vector3 NEGATIVE_UNIT_Y;
        static const Vector3 NEGATIVE_UNIT_Z;
        static const Vector3 UNIT_SCALE;
    };
}

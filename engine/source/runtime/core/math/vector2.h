#pragma once

#include "runtime/core/math/math_angle.h"

#include <cassert>
#include <cmath>

/**
 * vector2
 */
namespace VKernel
{
    class Vector2
    {

    public:
        float x {0.f}, y {0.f};

    public:
        // Constructor
        //-----------------------------------------------------------------------
        Vector2() = default;

        Vector2(float x_, float y_) : x(x_), y(y_) {}

        explicit Vector2(float scaler) : x(scaler), y(scaler) {}

        explicit Vector2(const float v[2]) : x(v[0]), y(v[1]) {}

        explicit Vector2(float* const r) : x(r[0]), y(r[1]) {}

        // get ptr
        //-----------------------------------------------------------------------
        float* ptr() { return &x; }

        const float* ptr() const { return &x; }

        // overloaded operator([], ==, !=, +-*/, +=-=*=/=)
        //-----------------------------------------------------------------------
        float operator[](size_t i) const
        {
            assert(i < 2);
            return (i == 0 ? x : y);
        }

        float& operator[](size_t i)
        {
            assert(i < 2);
            return (i == 0 ? x : y);
        }

        bool operator==(const Vector2& rhs) const { return (x == rhs.x && y == rhs.y); }

        bool operator!=(const Vector2& rhs) const { return (x != rhs.x || y != rhs.y); }

        Vector2 operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }

        Vector2 operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }

        Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

        Vector2 operator*(const Vector2& rhs) const { return Vector2(x * rhs.x, y * rhs.y); }

        Vector2 operator/(float scale) const
        {
            assert(scale != 0.0);

            float inv = 1.0f / scale;
            return Vector2(x * inv, y * inv);
        }

        Vector2 operator/(const Vector2& rhs) const { return Vector2(x / rhs.x, y / rhs.y); }

        const Vector2& operator+() const { return *this; }

        Vector2 operator-() const { return Vector2(-x, -y); }

        friend Vector2 operator*(float scalar, const Vector2& rhs) { return Vector2(scalar * rhs.x, scalar * rhs.y); }

        friend Vector2 operator/(float fScalar, const Vector2& rhs)
        {
            return Vector2(fScalar / rhs.x, fScalar / rhs.y);
        }

        friend Vector2 operator+(const Vector2& lhs, float rhs) { return Vector2(lhs.x + rhs, lhs.y + rhs); }

        friend Vector2 operator+(float lhs, const Vector2& rhs) { return Vector2(lhs + rhs.x, lhs + rhs.y); }

        friend Vector2 operator-(const Vector2& lhs, float rhs) { return Vector2(lhs.x - rhs, lhs.y - rhs); }

        friend Vector2 operator-(float lhs, const Vector2& rhs) { return Vector2(lhs - rhs.x, lhs - rhs.y); }

        Vector2& operator+=(const Vector2& rhs)
        {
            x += rhs.x;
            y += rhs.y;

            return *this;
        }

        Vector2& operator+=(float scalar)
        {
            x += scalar;
            y += scalar;

            return *this;
        }

        Vector2& operator-=(const Vector2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;

            return *this;
        }

        Vector2& operator-=(float scalar)
        {
            x -= scalar;
            y -= scalar;

            return *this;
        }

        Vector2& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;

            return *this;
        }

        Vector2& operator*=(const Vector2& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;

            return *this;
        }

        Vector2& operator/=(float scalar)
        {
            assert(scalar != 0.0);

            float inv = 1.0f / scalar;

            x *= inv;
            y *= inv;

            return *this;
        }

        Vector2& operator/=(const Vector2& rhs)
        {
            x /= rhs.x;
            y /= rhs.y;

            return *this;
        }
        bool operator<(const Vector2& rhs) const { return x < rhs.x && y < rhs.y; }
        
        bool operator>(const Vector2& rhs) const { return x > rhs.x && y > rhs.y; }

        // util function
        //-----------------------------------------------------------------------
        // length
        // Returns the length of the vector.
        float length() const { return std::hypot(x, y); }
        // Returns the square of the length(magnitude) of the vector.
        float squaredLength() const { return x * x + y * y; }
        // Returns the distance to another vector.
        float distance(const Vector2& rhs) const { return (*this - rhs).length(); }
        // Returns the square of the distance to another vector.
        float squaredDistance(const Vector2& rhs) const { return (*this - rhs).squaredLength(); }

        // Calculates the dot (scalar) product of this vector with another.
        float dotProduct(const Vector2& vec) const { return x * vec.x + y * vec.y; }

        // Normalizes the vector.
        float normalise()
        {
            float lengh = std::hypot(x, y);

            if (lengh > 0.0f)
            {
                float inv_length = 1.0f / lengh;
                x *= inv_length;
                y *= inv_length;
            }

            return lengh;
        }

        // get and set xy
        float getX() const { return x; }
        float getY() const { return y; }

        void setX(float value) { x = value; }
        void setY(float value) { y = value; }

        // Returns a vector at a point half way between this and the passed in vector.
        Vector2 midPoint(const Vector2& vec) const { return Vector2((x + vec.x) * 0.5f, (y + vec.y) * 0.5f); }

        // Sets this vector's components to the minimum of its own and the ones of the passed in vector.
        void makeFloor(const Vector2& cmp)
        {
            if (cmp.x < x)
                x = cmp.x;
            if (cmp.y < y)
                y = cmp.y;
        }
        // Sets this vector's components to the maximum of its own and the ones of the passed in vector.
        void makeCeil(const Vector2& cmp)
        {
            if (cmp.x > x)
                x = cmp.x;
            if (cmp.y > y)
                y = cmp.y;
        }

        // Generates a vector perpendicular to this vector (eg an 'up' vector).
        Vector2 perpendicular(void) const { return Vector2(-y, x); }

        // Calculates the 2 dimensional cross-product of 2 vectors, which results in a single floating point value which is 2 times the area of the triangle.
        float crossProduct(const Vector2& rhs) const { return x * rhs.y - y * rhs.x; }

        // Returns true if this vector is zero length. 
        bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y);
            return (sqlen < (Float_EPSILON * Float_EPSILON));
        }

        // As normalise, except that this vector is unaffected and the normalised vector is returned as a copy.
        Vector2 normalisedCopy(void) const
        {
            Vector2 ret = *this;
            ret.normalise();
            return ret;
        }

        // Calculates a reflection vector to the plane with the given normal
        Vector2 reflect(const Vector2& normal) const
        {
            return Vector2(*this - (2 * this->dotProduct(normal) * normal));
        }

        // Check whether this vector contains valid values
        bool isNaN() const { return Math::isNan(x) || Math::isNan(y); }

        // linear interpolation
        static Vector2 lerp(const Vector2& lhs, const Vector2& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

        // special value
        //-----------------------------------------------------------------------
        static const Vector2 ZERO;
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
        static const Vector2 NEGATIVE_UNIT_X;
        static const Vector2 NEGATIVE_UNIT_Y;
        static const Vector2 UNIT_SCALE;
    };

}

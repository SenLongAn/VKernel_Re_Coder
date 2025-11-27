#include "runtime/core/math/quaternion.h"
#include "runtime/core/math/matrix3.h"
#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/vector3.h"

namespace VKernel
{
    const Quaternion Quaternion::ZERO(0, 0, 0, 0);
    const Quaternion Quaternion::IDENTITY(1, 0, 0, 0);

    const float Quaternion::k_epsilon = 1e-03;

    Quaternion Quaternion::operator*(const Quaternion& rhs) const
    {
        return Quaternion(w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
                          w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                          w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
                          w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x);
    }

    void Quaternion::fromRotationMatrix(const Matrix3x3& rotation)
    {
        float trace = rotation[0][0] + rotation[1][1] + rotation[2][2];
        float root;

        if (trace > 0.0)
        {
            root = std::sqrt(trace + 1.0f); // 2w
            w    = 0.5f * root;
            root = 0.5f / root; 
            x    = (rotation[2][1] - rotation[1][2]) * root;
            y    = (rotation[0][2] - rotation[2][0]) * root;
            z    = (rotation[1][0] - rotation[0][1]) * root;
        }
        else
        {
            size_t s_iNext[3] = {1, 2, 0};
            size_t i          = 0;
            if (rotation[1][1] > rotation[0][0])
                i = 1;
            if (rotation[2][2] > rotation[i][i])
                i = 2;
            size_t j = s_iNext[i];
            size_t k = s_iNext[j];

            root              = std::sqrt(rotation[i][i] - rotation[j][j] - rotation[k][k] + 1.0f);
            float* apkQuat[3] = {&x, &y, &z};
            *apkQuat[i]       = 0.5f * root;
            root              = 0.5f / root;
            w                 = (rotation[k][j] - rotation[j][k]) * root;
            *apkQuat[j]       = (rotation[j][i] + rotation[i][j]) * root;
            *apkQuat[k]       = (rotation[k][i] + rotation[i][k]) * root;
        }
    }

    Vector3 Quaternion::toEulerAngle() const
    {
        Vector3 euler_angle;
        euler_angle.x = this->getPitch(false).valueDegrees();
        euler_angle.y = this->getRoll(false).valueDegrees();
        euler_angle.z = this->getYaw(false).valueDegrees();
        return euler_angle;
    }

    void Quaternion::fromEulerAngle(const Vector3& eulerAngle)
    {
        this->w = Math::cos(Math::degreesToRadians(eulerAngle.x / 2)) *
                                        Math::cos(Math::degreesToRadians(eulerAngle.y / 2)) *
                                        Math::cos(Math::degreesToRadians(eulerAngle.z / 2)) +
                                    Math::sin(Math::degreesToRadians(eulerAngle.x / 2)) *
                                        Math::sin(Math::degreesToRadians(eulerAngle.y / 2)) *
                                        Math::sin(Math::degreesToRadians(eulerAngle.z / 2));
        this->x = Math::sin(Math::degreesToRadians(eulerAngle.x / 2)) *
                                        Math::cos(Math::degreesToRadians(eulerAngle.y / 2)) *
                                        Math::cos(Math::degreesToRadians(eulerAngle.z / 2)) -
                                    Math::cos(Math::degreesToRadians(eulerAngle.x / 2)) *
                                        Math::sin(Math::degreesToRadians(eulerAngle.y / 2)) *
                                        Math::sin(Math::degreesToRadians(eulerAngle.z / 2));
        this->y = Math::cos(Math::degreesToRadians(eulerAngle.x / 2)) *
                                        Math::sin(Math::degreesToRadians(eulerAngle.y / 2)) *
                                        Math::cos(Math::degreesToRadians(eulerAngle.z / 2)) +
                                    Math::sin(Math::degreesToRadians(eulerAngle.x / 2)) *
                                        Math::cos(Math::degreesToRadians(eulerAngle.y / 2)) *
                                        Math::sin(Math::degreesToRadians(eulerAngle.z / 2));
        this->z = Math::cos(Math::degreesToRadians(eulerAngle.x / 2)) *
                                        Math::cos(Math::degreesToRadians(eulerAngle.y / 2)) *
                                        Math::sin(Math::degreesToRadians(eulerAngle.z / 2)) -
                                    Math::sin(Math::degreesToRadians(eulerAngle.x / 2)) *
                                        Math::sin(Math::degreesToRadians(eulerAngle.y / 2)) *
                                        Math::cos(Math::degreesToRadians(eulerAngle.z / 2));
        this->normalise();
    }

    void Quaternion::toRotationMatrix(Matrix3x3& kRot) const
    {
        float fTx  = x + x;  
        float fTy  = y + y;  
        float fTz  = z + z;  
        float fTwx = fTx * w;
        float fTwy = fTy * w;
        float fTwz = fTz * w;
        float fTxx = fTx * x;
        float fTxy = fTy * x;
        float fTxz = fTz * x;
        float fTyy = fTy * y;
        float fTyz = fTz * y;
        float fTzz = fTz * z;

        kRot[0][0] = 1.0f - (fTyy + fTzz); 
        kRot[0][1] = fTxy - fTwz;          
        kRot[0][2] = fTxz + fTwy;          
        kRot[1][0] = fTxy + fTwz;          
        kRot[1][1] = 1.0f - (fTxx + fTzz); 
        kRot[1][2] = fTyz - fTwx;          
        kRot[2][0] = fTxz - fTwy;          
        kRot[2][1] = fTyz + fTwx;          
        kRot[2][2] = 1.0f - (fTxx + fTyy); 
    }

    void Quaternion::toRotationMatrix(Matrix4x4& kRot) const
    {
        float fTx  = x + x;   
        float fTy  = y + y;   
        float fTz  = z + z;   
        float fTwx = fTx * w; 
        float fTwy = fTy * w; 
        float fTwz = fTz * w; 
        float fTxx = fTx * x; 
        float fTxy = fTy * x; 
        float fTxz = fTz * x; 
        float fTyy = fTy * y; 
        float fTyz = fTz * y; 
        float fTzz = fTz * z; 

        kRot[0][0] = 1.0f - (fTyy + fTzz);
        kRot[0][1] = fTxy - fTwz;         
        kRot[0][2] = fTxz + fTwy;         
        kRot[0][3] = 0;
        kRot[1][0] = fTxy + fTwz;         
        kRot[1][1] = 1.0f - (fTxx + fTzz);
        kRot[1][2] = fTyz - fTwx;         
        kRot[1][3] = 0;
        kRot[2][0] = fTxz - fTwy;         
        kRot[2][1] = fTyz + fTwx;         
        kRot[2][2] = 1.0f - (fTxx + fTyy);
        kRot[2][3] = 0;
        kRot[3][0] = 0;
        kRot[3][1] = 0;
        kRot[3][2] = 0;
        kRot[3][3] = 1;
    }

    void Quaternion::fromAngleAxis(const Radian& angle, const Vector3& axis)
    {
        Radian half_angle(0.5 * angle);
        float  sin_v = Math::sin(half_angle);
        w            = Math::cos(half_angle);
        x            = sin_v * axis.x;
        y            = sin_v * axis.y;
        z            = sin_v * axis.z;
    }

    Quaternion Quaternion::getQuaternionFromAngleAxis(const Radian& angle, const Vector3& axis)
    {
        Quaternion q;
        q.fromAngleAxis(angle, axis);
        return q;
    }

    void Quaternion::fromDirection(const Vector3& direction, const Vector3& up_direction)
    {
        Vector3 forward_direction = direction;
        forward_direction.z       = 0.0f;
        forward_direction.normalise();

        Vector3 left_direction = up_direction.crossProduct(forward_direction);

        fromAxes(left_direction, -forward_direction, up_direction);
        normalise();
    }

    Quaternion Quaternion::getQuaternionFromDirection(const Vector3& direction, const Vector3& up_direction)
    {
        Quaternion object_orientation;
        object_orientation.fromDirection(direction, up_direction);
        return object_orientation;
    }

    void Quaternion::toAngleAxis(Radian& angle, Vector3& axis) const
    {
        float sqr_len = x * x + y * y + z * z;
        if (sqr_len > 0.0)
        {
            angle         = 2.0 * Math::acos(w);
            float inv_len = Math::invSqrt(sqr_len);
            axis.x        = x * inv_len;
            axis.y        = y * inv_len;
            axis.z        = z * inv_len;
        }
        else
        {
            angle  = Radian(0.0);
            axis.x = 1.0;
            axis.y = 0.0;
            axis.z = 0.0;
        }
    }

    void Quaternion::fromAxes(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
    {
        Matrix3x3 rot;

        rot[0][0] = xaxis.x;
        rot[1][0] = xaxis.y;
        rot[2][0] = xaxis.z;

        rot[0][1] = yaxis.x;
        rot[1][1] = yaxis.y;
        rot[2][1] = yaxis.z;

        rot[0][2] = zaxis.x;
        rot[1][2] = zaxis.y;
        rot[2][2] = zaxis.z;

        fromRotationMatrix(rot);
    }

    Vector3 Quaternion::xAxis() const
    {
        float ty  = 2.0f * y;
        float tz  = 2.0f * z;
        float twy = ty * w;
        float twz = tz * w;
        float txy = ty * x;
        float txz = tz * x;
        float tyy = ty * y;
        float tzz = tz * z;

        return Vector3(1.0f - (tyy + tzz), txy + twz, txz - twy);
    }

    Vector3 Quaternion::yAxis() const
    {
        float tx  = 2.0f * x;
        float ty  = 2.0f * y;
        float tz  = 2.0f * z;
        float twx = tx * w;
        float twz = tz * w;
        float txx = tx * x;
        float txy = ty * x;
        float tyz = tz * y;
        float tzz = tz * z;

        return Vector3(txy - twz, 1.0f - (txx + tzz), tyz + twx);
    }

    Vector3 Quaternion::zAxis() const
    {
        float tx  = 2.0f * x;
        float ty  = 2.0f * y;
        float tz  = 2.0f * z;
        float twx = tx * w;
        float twy = ty * w;
        float txx = tx * x;
        float txz = tz * x;
        float tyy = ty * y;
        float tyz = tz * y;

        return Vector3(txz + twy, tyz - twx, 1.0f - (txx + tyy));
    }

    void Quaternion::toAxes(Vector3& xaxis, Vector3& yaxis, Vector3& zaxis) const
    {
        Matrix3x3 rot;

        toRotationMatrix(rot);

        xaxis.x = rot[0][0];
        xaxis.y = rot[1][0];
        xaxis.z = rot[2][0];

        yaxis.x = rot[0][1];
        yaxis.y = rot[1][1];
        yaxis.z = rot[2][1];

        zaxis.x = rot[0][2];
        zaxis.y = rot[1][2];
        zaxis.z = rot[2][2];
    }

    Vector3 Quaternion::operator*(const Vector3& v) const
    {
        Vector3 uv, uuv;
        Vector3 qvec(x, y, z);
        uv  = qvec.crossProduct(v);
        uuv = qvec.crossProduct(uv);
        uv *= (2.0f * w);
        uuv *= 2.0f;

        return v + uv + uuv;
    }

    Radian Quaternion::getYaw(bool reproject_axis) const
    {
        if (reproject_axis)
        {
            float ty  = 2.0f * y;
            float tz  = 2.0f * z;
            float twz = tz * w;
            float txy = ty * x;
            float tyy = ty * y;
            float tzz = tz * z;

            return Radian(Math::atan2(txy + twz, 1.0f - (tyy + tzz)));
        }
        else
        {
            return Radian(Math::atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z));
        }
    }

    Radian Quaternion::getPitch(bool reproject_axis) const
    {
        if (reproject_axis)
        {
            float tx = 2.0f * x;
            float tz  = 2.0f * z;
            float twx = tx * w;
            float txx = tx * x;
            float tyz = tz * y;
            float tzz = tz * z;

            return Radian(Math::atan2(tyz + twx, 1.0f - (txx + tzz)));
        }
        else
        {
            return Radian(Math::atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z));
        }
    }

    Radian Quaternion::getRoll(bool reproject_axis) const
    {
        if (reproject_axis)
        {
            float tx  = 2.0f * x;
            float ty  = 2.0f * y;
            float tz  = 2.0f * z;
            float twy = ty * w;
            float txx = tx * x;
            float txz = tz * x;
            float tyy = ty * y;

            return Radian(Math::atan2(txz + twy, 1.0f - (txx + tyy)));
        }
        else
        {
            return Radian(Math::asin(-2 * (x * z - w * y)));
        }
    }

    Quaternion Quaternion::sLerp(float t, const Quaternion& kp, const Quaternion& kq, bool shortest_path)
    {
        float      cos_v = kp.dot(kq);
        Quaternion kt;

        if (cos_v < 0.0f && shortest_path)
        {
            cos_v = -cos_v;
            kt    = -kq;
        }
        else
        {
            kt = kq;
        }

        if (Math::abs(cos_v) < 1 - k_epsilon)
        {
            float  sin_v   = Math::sqrt(1 - Math::sqr(cos_v));
            Radian angle   = Math::atan2(sin_v, cos_v);
            float  inv_sin = 1.0f / sin_v;
            float  coeff0  = Math::sin((1.0f - t) * angle) * inv_sin;
            float  coeff1  = Math::sin(t * angle) * inv_sin;
            return coeff0 * kp + coeff1 * kt;
        }
        else
        {
            Quaternion r = (1.0f - t) * kp + t * kt;
            r.normalise();
            return r;
        }
    }

    Quaternion Quaternion::nLerp(float t, const Quaternion& kp, const Quaternion& kq, bool shortest_path)
    {
        Quaternion result;
        float      cos_value = kp.dot(kq);
        if (cos_value < 0.0f && shortest_path)
        {
            result = kp + t * ((-kq) - kp);
        }
        else
        {
            result = kp + t * (kq - kp);
        }
        result.normalise();
        return result;
    }
}

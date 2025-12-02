#pragma once

#include "runtime/core/math/vector2.h"
#include "runtime/core/math/vector3.h"

namespace VKernel
{
    class CameraPose
    {
    public:
        Vector3 m_position;
        Vector3 m_target;
        Vector3 m_up;
    };

    class CameraConfig
    {
    public:
        CameraPose m_pose;
        Vector2 m_aspect;
        float m_z_far;
        float m_z_near;
    };
}
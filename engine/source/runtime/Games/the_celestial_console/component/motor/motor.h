#pragma once

#include "runtime/core/meta/reflection/reflection.h"

namespace VKernel
{
    REFLECTION_TYPE(MotorComponentRes)
    CLASS(MotorComponentRes, Fields)
    {
        REFLECTION_BODY(MotorComponentRes);

    public:
        MotorComponentRes() = default;
        ~MotorComponentRes() {}

        float m_move_speed {0.f};
        float m_jump_height {0.f};
        float m_max_move_speed_ratio {0.f};
        float m_max_sprint_speed_ratio {0.f};
        float m_move_acceleration {0.f};
        float m_sprint_acceleration {0.f};
    };
} // namespace VKernel
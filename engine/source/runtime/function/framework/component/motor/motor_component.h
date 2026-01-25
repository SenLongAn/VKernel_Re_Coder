#pragma once

#include "runtime/function/framework/component/component.h"

#include "runtime/core/math/vector3.h"

namespace VKernel
{
    REFLECTION_TYPE(MotorComponent)
    CLASS(MotorComponent : public Component, WhiteListFields, WhiteListMethods)
    {
        REFLECTION_BODY(MotorComponent)

    public:
        // load
        MotorComponent() = default;
        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        ~MotorComponent() override;

        // tick
        void tick(float delta_time) override;
        void tickPlayerMotor(float delta_time);

        // get
        const Vector3& getTargetPosition() const { return m_target_position; }
        bool           getIsMoving() const { return m_is_moving; }

    private:
        bool m_is_moving {false};

        Vector3 m_target_position; ///< target position

        float m_move_speed_ratio {0.f}; ///< move speed

        Vector3 m_desired_horizontal_move_direction; ///< move direction

        Vector3 m_desired_displacement; ///< move vector

    private:
        void calculatedDesiredHorizontalMoveSpeed(unsigned int command, float delta_time);
        void calculatedDesiredVerticalMoveSpeed(unsigned int command, float delta_time);
        void calculatedDesiredMoveDirection(unsigned int command, const Quaternion& object_rotation);
        void calculateDesiredDisplacement(float delta_time);
        void calculateTargetPosition(const Vector3&& current_position);
    };
} // namespace VKernel
#pragma once

#include "runtime/function/framework/component/component.h"
#include "runtime/resource/res_type/components/motor.h"

#include "runtime/core/math/vector3.h"

namespace Games
{
    enum class JumpState : unsigned char
    {
        idle,
        rising,
        falling
    };

    REFLECTION_TYPE(MotorComponent)
    CLASS(MotorComponent : public VKernel::Component, WhiteListFields, WhiteListMethods)
    {
        REFLECTION_BODY(MotorComponent)
    public:
        META(Enable)
        VKernel::MotorComponentRes m_motor_res; ///< value

    public:
        // load
        MotorComponent() = default;
        void postLoadResource(std::weak_ptr<VKernel::GObject> parent_object) override;

        ~MotorComponent() override;

        // tick
        void tick(float delta_time) override;
        void tickPlayerMotor(float delta_time);

        // get
        const VKernel::Vector3&    getTargetPosition() const { return m_target_position; }
        bool                       getIsMoving() const { return m_is_moving; }
        const VKernel::Quaternion& getTargetRotation() const { return m_character_rotation; }

    private:
        VKernel::Vector3 m_target_position; ///< target position

        // move
        bool                m_is_moving {false};
        float               m_move_speed_ratio {0.f};            ///< move speed
        VKernel::Vector3    m_desired_horizontal_move_direction; ///< move direction
        VKernel::Vector3    m_desired_displacement;              ///< move vector
        VKernel::Quaternion m_character_rotation;

        // jump
        JumpState m_jump_state {JumpState::idle};      ///< state
        float     m_vertical_move_speed {0.f};         ///< speed
        float     m_jump_horizontal_speed_ratio {0.f}; ///< ratio

    private:
        void calculatedDesiredHorizontalMoveSpeed(unsigned int command, float delta_time);
        void calculatedDesiredVerticalMoveSpeed(unsigned int command, float delta_time);
        void calculatedDesiredMoveDirection(unsigned int command, const VKernel::Quaternion& object_rotation);
        void calculateDesiredDisplacement(float delta_time);
        void calculateTargetPosition(const VKernel::Vector3&& current_position);
    };
} // namespace Games
#pragma once

#include "runtime/function/character/character.h"

namespace VKernel
{
    class Character;
    class GObject;
} // namespace VKernel

namespace Games
{
    class ControlCabin : public VKernel::Character, public std::enable_shared_from_this<ControlCabin>
    {
        CHARACTER_CLASS(ControlCabin)

    public:
        ControlCabin(std::shared_ptr<VKernel::GObject> character_object);

        void tick(float delta_time) override;

        // set
        void setPosition(const VKernel::Vector3& position) { m_position = position; }
        void setRotation(const VKernel::Quaternion& rotation) { m_rotation = rotation; }

        // get
        const VKernel::Vector3&    getPosition() const { return m_position; }
        const VKernel::Quaternion& getRotation() const { return m_rotation; }

    private:
        VKernel::Vector3    m_position; ///< last frame position
        VKernel::Quaternion m_rotation; ///< last frame rotation
    };
} // namespace Games

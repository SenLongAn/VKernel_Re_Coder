#pragma once

#include "runtime/core/math/transform.h"
#include "runtime/function/framework/object/object.h"

namespace VKernel
{
    class Character
    {
    public:
        Character(std::shared_ptr<GObject> character_object);

        // set
        void setObject(std::shared_ptr<GObject> gobject);
        void setPosition(const Vector3& position) { m_position = position; }
        void setRotation(const Quaternion& rotation) { m_rotation = rotation; }

        // get
        GObjectID              getObjectID() const;
        std::weak_ptr<GObject> getObject() const { return m_character_object; }
        const Vector3&         getPosition() const { return m_position; }
        const Quaternion&      getRotation() const { return m_rotation; }

        void tick(float delta_time); ///< tick

    private:
        std::shared_ptr<GObject> m_character_object; ///< object

        Vector3    m_position; ///< last frame position
        Quaternion m_rotation; ///< last frame rotation
    };
} // namespace VKernel
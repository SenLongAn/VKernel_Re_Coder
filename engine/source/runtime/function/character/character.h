#pragma once

#include "runtime/function/framework/object/object.h"

namespace VKernel
{
    class Character
    {
    public:
        Character(std::shared_ptr<GObject> character_object);

        // set
        void setObject(std::shared_ptr<GObject> gobject);

        // get
        GObjectID getObjectID() const;

    private:
        std::shared_ptr<GObject> m_character_object;
    };
} // namespace VKernel
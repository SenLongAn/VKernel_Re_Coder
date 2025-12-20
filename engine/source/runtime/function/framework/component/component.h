#pragma once
#include "runtime/core/meta/reflection/reflection.h"

#include <memory>

/**
 * component base class
 */
namespace VKernel
{
    class GObject;

    REFLECTION_TYPE(Component)
    CLASS(Component, WhiteListFields)
    {
        REFLECTION_BODY(Component)
    public:
        Component() = default;  ///< Constructor
        virtual ~Component() {} ///< Destructor

        virtual void postLoadResource(std::weak_ptr<GObject> parent_object) { m_parent_object = parent_object; }

        virtual void tick(float delta_time) {}; ///< tick

    protected:
        std::weak_ptr<GObject> m_parent_object; ///< parent object
    };
} // namespace VKernel
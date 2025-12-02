#pragma once

#include <memory>

/**
 * component base class
 */
namespace VKernel
{
    class GObject;

    class Component
    {

    public:
        Component() = default; ///< Constructor
        virtual ~Component() {} ///< Destructor

        virtual void postLoadResource(std::weak_ptr<GObject> parent_object) { m_parent_object = parent_object; }

        virtual void tick(float delta_time) {}; ///< tick

    protected:
        std::weak_ptr<GObject> m_parent_object; ///< parent object
    };
}
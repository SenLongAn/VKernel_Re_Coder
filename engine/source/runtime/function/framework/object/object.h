#pragma once

#include <memory>

/**
 * Game Object
 */
namespace VKernel
{
    class Component;

    class GObject : public std::enable_shared_from_this<GObject> ///< weak_from_this
    {

    public:
        virtual ~GObject(); ///< Destructor

        bool load(); ///< load

        virtual void tick(float delta_time); ///< tick

    protected:
        std::shared_ptr<Component> m_loaded_component; ///< all loaded component
    };
}
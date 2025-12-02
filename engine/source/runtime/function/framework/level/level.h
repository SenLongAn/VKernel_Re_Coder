#pragma once

#include <memory>

/**
 * level
 */
namespace VKernel
{
    class GObject;

    class Level
    {

    public:
        virtual ~Level(){}; ///< Destructor

        bool load(); ///< load

        void tick(float delta_time); ///< tick

    protected:
        std::shared_ptr<GObject> m_gobjects; ///< all loaded objects

        bool m_is_loaded {false}; ///< Has the level been loaded
    };
}
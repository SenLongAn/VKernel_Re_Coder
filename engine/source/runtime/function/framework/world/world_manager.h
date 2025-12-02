#pragma once

#include <memory>

/**
 * world
 */
namespace VKernel
{
    class Level;

    class WorldManager
    {

    public:
        virtual ~WorldManager(); ///< Destructor
        void clear(); /// clear

        void initialize();           ///< init
        void tick(float delta_time); ///< tick

    private:
        bool m_is_world_loaded{false};               ///< Has the world been loaded
        std::shared_ptr<Level> m_loaded_level;       ///< all loaded levels
        std::weak_ptr<Level> m_current_active_level; ///< active level

        bool loadWorld(); ///< load world
    };
}
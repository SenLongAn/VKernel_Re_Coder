#pragma once

#include "runtime/resource/res_type/common/world.h"

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
        void clear();            /// clear

        void initialize();           ///< init
        void tick(float delta_time); ///< tick

        void reloadCurrentLevel(); ///< reload current level
        void saveCurrentLevel();   ///< save current level

        // get
        std::weak_ptr<Level> getCurrentActiveLevel() const { return m_current_active_level; }

    private:
        bool m_is_world_loaded {false}; ///< Has the world been loaded

        std::string                                             m_current_world_url; ///< world url
        std::unordered_map<std::string, std::shared_ptr<Level>> m_loaded_levels;     ///< all levels: level url, level
        std::weak_ptr<Level>                                    m_current_active_level; ///< current active level

    private:
        bool loadWorld(const std::string& world_url); ///< load world
        bool loadLevel(const std::string& level_url);
    };
} // namespace VKernel
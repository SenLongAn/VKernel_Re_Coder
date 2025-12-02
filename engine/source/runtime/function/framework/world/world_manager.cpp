#include "runtime/function/framework/world/world_manager.h"

#include "runtime/function/framework/level/level.h"   

namespace VKernel
{
    WorldManager::~WorldManager() { clear(); }

    void WorldManager::clear()
    {
        m_current_active_level.reset();

        m_is_world_loaded = false;
    }

    void WorldManager::initialize()
    {
        m_is_world_loaded = false;
    }
    
    void WorldManager::tick(float delta_time)
    {

        if (!m_is_world_loaded)
        {
            loadWorld();
        }

        // tick the active level
        std::shared_ptr<Level> active_level = m_current_active_level.lock();
        if (active_level)
        {
            active_level->tick(delta_time);
        }
    }
    
    bool WorldManager::loadWorld()
    {
        // load level
        m_loaded_level = std::make_shared<Level>();
        const bool is_level_load_success = m_loaded_level->load();
        if (is_level_load_success == false)
        {
            return false;
        }

        // set
        m_current_active_level = m_loaded_level;
        m_is_world_loaded = true;

        return true;
    }
}
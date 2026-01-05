#include "runtime/function/framework/world/world_manager.h"

#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/config_manager/config_manager.h"

#include "runtime/function/framework/level/level.h"
#include "runtime/function/global/global_context.h"

#include "_generated/serializer/all_serializer.h"
#include "world_manager.h"

namespace VKernel
{
    WorldManager::~WorldManager() { clear(); }

    void WorldManager::clear()
    {
        // unload all loaded levels
        for (auto level_pair : m_loaded_levels)
        {
            level_pair.second->unload();
        }
        m_loaded_levels.clear();

        m_current_active_level.reset();

        // clear world
        m_current_world_url.clear();
        m_is_world_loaded = false;
    }

    void WorldManager::initialize()
    {
        m_is_world_loaded = false;

        m_current_world_url = g_runtime_global_context.m_config_manager->getDefaultWorldUrl();
    }

    void WorldManager::tick(float delta_time)
    {
        // load world
        if (!m_is_world_loaded)
        {
            loadWorld(m_current_world_url);
        }

        // tick the active level
        std::shared_ptr<Level> active_level = m_current_active_level.lock();
        if (active_level)
        {
            active_level->tick(delta_time);
        }
    }

    void WorldManager::reloadCurrentLevel()
    {
        // find current active level
        auto active_level = m_current_active_level.lock();
        if (active_level == nullptr)
        {
            throw std::runtime_error("current level is nil");
            return;
        }

        // clear current active level
        const std::string level_url = active_level->getLevelResUrl();
        active_level->unload();
        m_loaded_levels.erase(level_url);

        // reload this level
        const bool is_load_success = loadLevel(level_url);
        if (!is_load_success)
        {
            throw std::runtime_error("load level failed");
            return;
        }

        // Validation
        auto iter = m_loaded_levels.find(level_url);
        if (iter == m_loaded_levels.end())
        {
            throw std::runtime_error("load level failed");
            return;
        }

        // reset current active level
        m_current_active_level = iter->second;
    }

    bool WorldManager::loadWorld(const std::string& world_url)
    {
        // load world
        WorldRes   world_res;
        const bool is_world_load_success = g_runtime_global_context.m_asset_manager->loadAsset(world_url, world_res);
        if (!is_world_load_success)
        {
            return false;
        }

        // load default level
        const bool is_level_load_success = loadLevel(world_res.m_default_level_url);
        if (!is_level_load_success)
        {
            return false;
        }
        auto iter              = m_loaded_levels.find(world_res.m_default_level_url);
        m_current_active_level = iter->second;

        // Already loaded
        m_is_world_loaded = true;

        return true;
    }

    bool WorldManager::loadLevel(const std::string& level_url)
    {
        // load level
        std::shared_ptr<Level> level = std::make_shared<Level>();

        m_current_active_level = level;

        const bool is_level_load_success = level->load(level_url);
        if (is_level_load_success == false)
        {
            return false;
        }

        m_loaded_levels.emplace(level_url, level);

        return true;
    }
} // namespace VKernel

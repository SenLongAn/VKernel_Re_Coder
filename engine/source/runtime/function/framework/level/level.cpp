#include "runtime/function/framework/level/level.h"

#include "runtime/function/framework/object/object.h"
#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/res_type/common/level.h"

#include "runtime/function/framework/object/object.h"
#include "runtime/function/global/global_context.h"

#include <iostream>
#include <limits>

namespace VKernel
{
    bool Level::load(const std::string& level_res_url)
    {
        m_level_res_url = level_res_url;

        std::cout << level_res_url << std::endl;
        // load level
        LevelRes   level_res;
        const bool is_load_success = g_runtime_global_context.m_asset_manager->loadAsset(level_res_url, level_res);
        if (is_load_success == false)
        {
            return false;
        }

        // load object
        for (const ObjectInstanceRes& object_instance_res : level_res.m_objects)
        {
            createObject(object_instance_res);
        }

        m_is_loaded = true;

        return true;
    }

    void Level::unload() { clear(); }

    void Level::tick(float delta_time)
    {
        if (!m_is_loaded)
        {
            return;
        }

        // Update All gobjcets
        for (const auto& id_object_pair : m_gobjects)
        {
            if (id_object_pair.second)
            {
                id_object_pair.second->tick(delta_time);
            }
        }
    }

    GObjectID Level::createObject(const ObjectInstanceRes& object_instance_res)
    {
        GObjectID object_id = ObjectIDAllocator::alloc(); ///< alloctor guid

        std::shared_ptr<GObject> gobject   = std::make_shared<GObject>(object_id); ///< create new GO
        bool                     is_loaded = gobject->load(object_instance_res);   ///< load GO
        if (is_loaded)
        {
            m_gobjects.emplace(object_id, gobject); ///< add to map
        }
        else
        {
            throw std::runtime_error("oading object failed");
            return k_invalid_gobject_id;
        }

        return object_id;
    }

    bool Level::save()
    {
        // Data Struct
        LevelRes                        output_level_res;
        const size_t                    object_cout    = m_gobjects.size();
        std::vector<ObjectInstanceRes>& output_objects = output_level_res.m_objects;
        output_objects.resize(object_cout);

        // Iterate over all objects in the level
        size_t object_index = 0;
        for (const auto& id_object_pair : m_gobjects)
        {
            if (id_object_pair.second)
            {
                id_object_pair.second->save(output_objects[object_index]); ///< obj save
                ++object_index;
            }
        }

        const bool is_save_success =
            g_runtime_global_context.m_asset_manager->saveAsset(output_level_res, m_level_res_url); ///< Serialization

        if (is_save_success == false)
        {
            throw std::runtime_error("failed to save");
        }

        return is_save_success;
    }

    void Level::clear() { m_gobjects.clear(); }
} // namespace VKernel

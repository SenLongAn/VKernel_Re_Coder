#pragma once

#include "runtime/function/framework/object/object_id_allocator.h"
#include "runtime/resource/res_type/common/level.h"

#include <memory>
#include <string>
#include <unordered_map>

/**
 * level
 */
namespace VKernel
{
    class Character;
    class GObject;
    class ObjectInstanceRes;

    using LevelObjectsMap = std::unordered_map<uint8_t, std::shared_ptr<GObject>>;

    class Level
    {

    public:
        virtual ~Level() {}; ///< Destructor

        bool load(const std::string& level_res_url); ///< load
        void unload();                               ///< un load

        void tick(float delta_time); ///< tick

        GObjectID createObject(const ObjectInstanceRes& object_instance_res);

        // get
        const std::string&       getLevelResUrl() const { return m_level_res_url; } ///< get level url
        std::weak_ptr<Character> getCurrentActiveCharacter() const { return m_current_active_character; }

        bool save(); ///< save

        // get
        std::weak_ptr<GObject> getGObjectByID(GObjectID go_id) const;
        const LevelObjectsMap& getAllGObjects() const { return m_gobjects; }

    protected:
        std::string m_level_res_url; ///< level url

        LevelRes level_res;

        LevelObjectsMap                                           m_gobjects; ///< all loaded objects: id, object
        std::unordered_map<std::string, std::shared_ptr<GObject>> m_go;       ///< name, GO

        bool m_is_loaded {false}; ///< Has the level been loaded

        std::shared_ptr<Character> m_current_active_character; ///< character

    protected:
        void clear(); ///< clear
    };
} // namespace VKernel
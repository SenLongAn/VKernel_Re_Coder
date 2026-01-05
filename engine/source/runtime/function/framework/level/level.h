#pragma once

#include <memory>
#include <string>
#include <unordered_map>

/**
 * level
 */
namespace VKernel
{
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

        void createObject(const ObjectInstanceRes& object_instance_res);

        const std::string& getLevelResUrl() const { return m_level_res_url; } ///< get level url

        bool save(); ///< save

    protected:
        std::string m_level_res_url; ///< level url

        LevelObjectsMap m_gobjects; ///< all loaded objects: id, object

        bool m_is_loaded {false}; ///< Has the level been loaded
    protected:
        void clear(); ///< clear
    };
} // namespace VKernel
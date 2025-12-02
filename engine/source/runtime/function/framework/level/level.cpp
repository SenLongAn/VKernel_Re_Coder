#include "runtime/function/framework/level/level.h"

#include "runtime/function/framework/object/object.h"

namespace VKernel
{
    bool Level::load()
    {
        m_gobjects = std::make_shared<GObject>();
        bool is_loaded = m_gobjects->load();
        if (is_loaded == false)
        {
            return false;
        }
        
        m_is_loaded = true;

        return true;
    }
    
    void Level::tick(float delta_time)
    {
        if (!m_is_loaded)
        {
            return;
        }

        m_gobjects->tick(delta_time);
    }
}


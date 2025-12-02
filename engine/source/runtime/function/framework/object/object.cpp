#include "runtime/function/framework/object/object.h"

#include "runtime/function/framework/component/camera/camera_component.h"

namespace VKernel
{
    GObject::~GObject()
    {
        m_loaded_component.reset();
    }

    bool GObject::load()
    {
        m_loaded_component = std::make_shared<CameraComponent>();
        m_loaded_component->postLoadResource(weak_from_this());
        
        return true;
    }

    void GObject::tick(float delta_time)
    {
        m_loaded_component->tick(delta_time);
    }
}
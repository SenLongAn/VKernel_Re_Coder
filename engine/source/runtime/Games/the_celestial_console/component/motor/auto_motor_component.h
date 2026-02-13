#pragma once

#include "runtime/function/framework/component/component.h"

namespace Games
{
    REFLECTION_TYPE(AutoMotorComponent)
    CLASS(AutoMotorComponent : public VKernel::Component, WhiteListFields, WhiteListMethods)
    {
        REFLECTION_BODY(AutoMotorComponent)
    public:
        // load
        void postLoadResource(std::weak_ptr<VKernel::GObject> parent_object) override;

        // tick
        void tick(float delta_time) override;
    };
} // namespace Games
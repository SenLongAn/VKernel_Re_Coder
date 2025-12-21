#pragma once

#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/resource/res_type/data/camera_config.h"

namespace VKernel
{

    REFLECTION_TYPE(GlobalRenderingRes)
    CLASS(GlobalRenderingRes, Fields)
    {
        REFLECTION_BODY(GlobalRenderingRes);

    public:
        CameraConfig m_camera_config;
    };
} // namespace VKernel
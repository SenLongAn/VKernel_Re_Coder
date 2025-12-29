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

    class SkyBoxIrradianceMap
    {
    public:
        std::string m_negative_x_map;
        std::string m_positive_x_map;
        std::string m_negative_y_map;
        std::string m_positive_y_map;
        std::string m_negative_z_map;
        std::string m_positive_z_map;
    };

    class SkyBoxSpecularMap ///< skybox image path
    {
    public:
        std::string m_negative_x_map;
        std::string m_positive_x_map;
        std::string m_negative_y_map;
        std::string m_positive_y_map;
        std::string m_negative_z_map;
        std::string m_positive_z_map;
    };

} // namespace VKernel
#pragma once
#include "runtime/resource/res_type/data/camera_config.h"

namespace VKernel{
    template<>
    Json Serializer::write(const CameraPose& instance);
    template<>
    CameraPose& Serializer::read(const Json& json_context, CameraPose& instance);
    template<>
    Json Serializer::write(const CameraConfig& instance);
    template<>
    CameraConfig& Serializer::read(const Json& json_context, CameraConfig& instance);
}//namespace


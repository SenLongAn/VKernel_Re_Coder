#pragma once
#include "runtime/resource/res_type/data/camera_config.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::CameraPose& instance);
    template<>
    VKernel::CameraPose& Serializer::read(const Json& json_context, VKernel::CameraPose& instance);
    template<>
    Json Serializer::write(const VKernel::CameraConfig& instance);
    template<>
    VKernel::CameraConfig& Serializer::read(const Json& json_context, VKernel::CameraConfig& instance);
}//namespace


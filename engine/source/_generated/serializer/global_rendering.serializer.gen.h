#pragma once
#include "runtime/resource/res_type/global/global_rendering.h"

namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::SkyBoxIrradianceMap& instance);
    template<>
    VKernel::SkyBoxIrradianceMap& Serializer::read(const Json& json_context, VKernel::SkyBoxIrradianceMap& instance);
    template<>
    Json Serializer::write(const VKernel::SkyBoxSpecularMap& instance);
    template<>
    VKernel::SkyBoxSpecularMap& Serializer::read(const Json& json_context, VKernel::SkyBoxSpecularMap& instance);
    template<>
    Json Serializer::write(const VKernel::DirectionalLight& instance);
    template<>
    VKernel::DirectionalLight& Serializer::read(const Json& json_context, VKernel::DirectionalLight& instance);
    template<>
    Json Serializer::write(const VKernel::PointLights& instance);
    template<>
    VKernel::PointLights& Serializer::read(const Json& json_context, VKernel::PointLights& instance);
    template<>
    Json Serializer::write(const VKernel::GlobalRenderingRes& instance);
    template<>
    VKernel::GlobalRenderingRes& Serializer::read(const Json& json_context, VKernel::GlobalRenderingRes& instance);
}//namespace


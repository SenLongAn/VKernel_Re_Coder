#pragma once
#include "runtime/resource/res_type/global/global_rendering.h"

namespace VKernel{
    template<>
    Json Serializer::write(const SkyBoxIrradianceMap& instance);
    template<>
    SkyBoxIrradianceMap& Serializer::read(const Json& json_context, SkyBoxIrradianceMap& instance);
    template<>
    Json Serializer::write(const SkyBoxSpecularMap& instance);
    template<>
    SkyBoxSpecularMap& Serializer::read(const Json& json_context, SkyBoxSpecularMap& instance);
    template<>
    Json Serializer::write(const DirectionalLight& instance);
    template<>
    DirectionalLight& Serializer::read(const Json& json_context, DirectionalLight& instance);
    template<>
    Json Serializer::write(const PointLights& instance);
    template<>
    PointLights& Serializer::read(const Json& json_context, PointLights& instance);
    template<>
    Json Serializer::write(const GlobalRenderingRes& instance);
    template<>
    GlobalRenderingRes& Serializer::read(const Json& json_context, GlobalRenderingRes& instance);
}//namespace


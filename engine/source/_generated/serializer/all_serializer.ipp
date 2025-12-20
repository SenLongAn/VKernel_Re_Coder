#pragma once
#include "_generated/serializer/camera_component.serializer.gen.h"
#include "_generated/serializer/component.serializer.gen.h"
namespace VKernel{
    template<>
    Json Serializer::write(const CameraComponent& instance){
        Json::object  ret_context;
        auto&&  json_context_0 = Serializer::write(*(VKernel::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("temp", Serializer::write(instance.temp));
        return  Json(ret_context);
    }
    template<>
    CameraComponent& Serializer::read(const Json& json_context, CameraComponent& instance){
        assert(json_context.is_object());
        Serializer::read(json_context,*(VKernel::Component*)&instance);
        if(!json_context["temp"].is_null()){
            Serializer::read(json_context["temp"], instance.temp);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const Component& instance){
        Json::object  ret_context;
        
        
        return  Json(ret_context);
    }
    template<>
    Component& Serializer::read(const Json& json_context, Component& instance){
        assert(json_context.is_object());
        
        
        return instance;
    }

}


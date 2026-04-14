#pragma once
#include "_generated/serializer/camera.serializer.gen.h"
#include "_generated/serializer/level.serializer.gen.h"
#include "_generated/serializer/quaternion.serializer.gen.h"
#include "_generated/serializer/transform.serializer.gen.h"
#include "_generated/serializer/world.serializer.gen.h"
#include "_generated/serializer/vector3.serializer.gen.h"
#include "_generated/serializer/vector4.serializer.gen.h"
#include "_generated/serializer/object.serializer.gen.h"
#include "_generated/serializer/vector2.serializer.gen.h"
#include "_generated/serializer/matrix4.serializer.gen.h"
#include "_generated/serializer/auto_motor_component.serializer.gen.h"
#include "_generated/serializer/component.serializer.gen.h"
#include "_generated/serializer/render_object.serializer.gen.h"
#include "_generated/serializer/camera_component.serializer.gen.h"
#include "_generated/serializer/motor.serializer.gen.h"
#include "_generated/serializer/motor_component.serializer.gen.h"
#include "_generated/serializer/color.serializer.gen.h"
#include "_generated/serializer/axis_aligned.serializer.gen.h"
#include "_generated/serializer/mesh.serializer.gen.h"
#include "_generated/serializer/mesh_component.serializer.gen.h"
#include "_generated/serializer/camera_config.serializer.gen.h"
#include "_generated/serializer/transform_component.serializer.gen.h"
#include "_generated/serializer/material.serializer.gen.h"
#include "_generated/serializer/global_rendering.serializer.gen.h"
namespace VKernel{
    template<>
    Json Serializer::write(const VKernel::CameraParameter& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("fov", Serializer::write(instance.m_fov));
        return  Json(ret_context);
    }
    template<>
    VKernel::CameraParameter& Serializer::read(const Json& json_context, VKernel::CameraParameter& instance){
        assert(json_context.is_object());
        
        if(!json_context["fov"].is_null()){
            Serializer::read(json_context["fov"], instance.m_fov);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::FirstPersonCameraParameter& instance){
        Json::object  ret_context;
        auto&&  json_context_0 = Serializer::write(*(VKernel::CameraParameter*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("vertical_offset", Serializer::write(instance.m_vertical_offset));
        return  Json(ret_context);
    }
    template<>
    VKernel::FirstPersonCameraParameter& Serializer::read(const Json& json_context, VKernel::FirstPersonCameraParameter& instance){
        assert(json_context.is_object());
        Serializer::read(json_context,*(VKernel::CameraParameter*)&instance);
        if(!json_context["vertical_offset"].is_null()){
            Serializer::read(json_context["vertical_offset"], instance.m_vertical_offset);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::ThirdPersonCameraParameter& instance){
        Json::object  ret_context;
        auto&&  json_context_0 = Serializer::write(*(VKernel::CameraParameter*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("horizontal_offset", Serializer::write(instance.m_horizontal_offset));
        ret_context.insert_or_assign("vertical_offset", Serializer::write(instance.m_vertical_offset));
        return  Json(ret_context);
    }
    template<>
    VKernel::ThirdPersonCameraParameter& Serializer::read(const Json& json_context, VKernel::ThirdPersonCameraParameter& instance){
        assert(json_context.is_object());
        Serializer::read(json_context,*(VKernel::CameraParameter*)&instance);
        if(!json_context["horizontal_offset"].is_null()){
            Serializer::read(json_context["horizontal_offset"], instance.m_horizontal_offset);
        }
        if(!json_context["vertical_offset"].is_null()){
            Serializer::read(json_context["vertical_offset"], instance.m_vertical_offset);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::CameraComponentRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("first_camera", Serializer::write(instance.m_first_camera));
        ret_context.insert_or_assign("third_camera", Serializer::write(instance.m_third_camera));
        return  Json(ret_context);
    }
    template<>
    VKernel::CameraComponentRes& Serializer::read(const Json& json_context, VKernel::CameraComponentRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["first_camera"].is_null()){
            Serializer::read(json_context["first_camera"], instance.m_first_camera);
        }
        if(!json_context["third_camera"].is_null()){
            Serializer::read(json_context["third_camera"], instance.m_third_camera);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::LevelRes& instance){
        Json::object  ret_context;
        
        Json::array m_character_name_json;
        for (auto& item : instance.m_character_name){
            m_character_name_json.emplace_back(Serializer::write(item));
        }
        ret_context.insert_or_assign("character_name",m_character_name_json);
        
        ret_context.insert_or_assign("current_character_name", Serializer::write(instance.m_current_character_name));
        Json::array m_objects_json;
        for (auto& item : instance.m_objects){
            m_objects_json.emplace_back(Serializer::write(item));
        }
        ret_context.insert_or_assign("objects",m_objects_json);
        
        return  Json(ret_context);
    }
    template<>
    VKernel::LevelRes& Serializer::read(const Json& json_context, VKernel::LevelRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["character_name"].is_null()){
            assert(json_context["character_name"].is_array());
            Json::array array_m_character_name = json_context["character_name"].array_items();
            instance.m_character_name.resize(array_m_character_name.size());
            for (size_t index=0; index < array_m_character_name.size();++index){
                Serializer::read(array_m_character_name[index], instance.m_character_name[index]);
            }
        }
        if(!json_context["current_character_name"].is_null()){
            Serializer::read(json_context["current_character_name"], instance.m_current_character_name);
        }
        if(!json_context["objects"].is_null()){
            assert(json_context["objects"].is_array());
            Json::array array_m_objects = json_context["objects"].array_items();
            instance.m_objects.resize(array_m_objects.size());
            for (size_t index=0; index < array_m_objects.size();++index){
                Serializer::read(array_m_objects[index], instance.m_objects[index]);
            }
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::Quaternion& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("w", Serializer::write(instance.w));
        ret_context.insert_or_assign("x", Serializer::write(instance.x));
        ret_context.insert_or_assign("y", Serializer::write(instance.y));
        ret_context.insert_or_assign("z", Serializer::write(instance.z));
        return  Json(ret_context);
    }
    template<>
    VKernel::Quaternion& Serializer::read(const Json& json_context, VKernel::Quaternion& instance){
        assert(json_context.is_object());
        
        if(!json_context["w"].is_null()){
            Serializer::read(json_context["w"], instance.w);
        }
        if(!json_context["x"].is_null()){
            Serializer::read(json_context["x"], instance.x);
        }
        if(!json_context["y"].is_null()){
            Serializer::read(json_context["y"], instance.y);
        }
        if(!json_context["z"].is_null()){
            Serializer::read(json_context["z"], instance.z);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::Transform& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("position", Serializer::write(instance.m_position));
        ret_context.insert_or_assign("scale", Serializer::write(instance.m_scale));
        ret_context.insert_or_assign("euler_rotation", Serializer::write(instance.m_euler_rotation));
        ret_context.insert_or_assign("is_serialization", Serializer::write(instance.m_is_serialization));
        return  Json(ret_context);
    }
    template<>
    VKernel::Transform& Serializer::read(const Json& json_context, VKernel::Transform& instance){
        assert(json_context.is_object());
        
        if(!json_context["position"].is_null()){
            Serializer::read(json_context["position"], instance.m_position);
        }
        if(!json_context["scale"].is_null()){
            Serializer::read(json_context["scale"], instance.m_scale);
        }
        if(!json_context["euler_rotation"].is_null()){
            Serializer::read(json_context["euler_rotation"], instance.m_euler_rotation);
        }
        if(!json_context["is_serialization"].is_null()){
            Serializer::read(json_context["is_serialization"], instance.m_is_serialization);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::WorldRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("name", Serializer::write(instance.m_name));
        Json::array m_level_urls_json;
        for (auto& item : instance.m_level_urls){
            m_level_urls_json.emplace_back(Serializer::write(item));
        }
        ret_context.insert_or_assign("level_urls",m_level_urls_json);
        
        ret_context.insert_or_assign("default_level_url", Serializer::write(instance.m_default_level_url));
        return  Json(ret_context);
    }
    template<>
    VKernel::WorldRes& Serializer::read(const Json& json_context, VKernel::WorldRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["name"].is_null()){
            Serializer::read(json_context["name"], instance.m_name);
        }
        if(!json_context["level_urls"].is_null()){
            assert(json_context["level_urls"].is_array());
            Json::array array_m_level_urls = json_context["level_urls"].array_items();
            instance.m_level_urls.resize(array_m_level_urls.size());
            for (size_t index=0; index < array_m_level_urls.size();++index){
                Serializer::read(array_m_level_urls[index], instance.m_level_urls[index]);
            }
        }
        if(!json_context["default_level_url"].is_null()){
            Serializer::read(json_context["default_level_url"], instance.m_default_level_url);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::Vector3& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("x", Serializer::write(instance.x));
        ret_context.insert_or_assign("y", Serializer::write(instance.y));
        ret_context.insert_or_assign("z", Serializer::write(instance.z));
        return  Json(ret_context);
    }
    template<>
    VKernel::Vector3& Serializer::read(const Json& json_context, VKernel::Vector3& instance){
        assert(json_context.is_object());
        
        if(!json_context["x"].is_null()){
            Serializer::read(json_context["x"], instance.x);
        }
        if(!json_context["y"].is_null()){
            Serializer::read(json_context["y"], instance.y);
        }
        if(!json_context["z"].is_null()){
            Serializer::read(json_context["z"], instance.z);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::Vector4& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("x", Serializer::write(instance.x));
        ret_context.insert_or_assign("y", Serializer::write(instance.y));
        ret_context.insert_or_assign("z", Serializer::write(instance.z));
        ret_context.insert_or_assign("w", Serializer::write(instance.w));
        return  Json(ret_context);
    }
    template<>
    VKernel::Vector4& Serializer::read(const Json& json_context, VKernel::Vector4& instance){
        assert(json_context.is_object());
        
        if(!json_context["x"].is_null()){
            Serializer::read(json_context["x"], instance.x);
        }
        if(!json_context["y"].is_null()){
            Serializer::read(json_context["y"], instance.y);
        }
        if(!json_context["z"].is_null()){
            Serializer::read(json_context["z"], instance.z);
        }
        if(!json_context["w"].is_null()){
            Serializer::read(json_context["w"], instance.w);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::ComponentDefinitionRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("type_name", Serializer::write(instance.m_type_name));
        ret_context.insert_or_assign("component", Serializer::write(instance.m_component));
        return  Json(ret_context);
    }
    template<>
    VKernel::ComponentDefinitionRes& Serializer::read(const Json& json_context, VKernel::ComponentDefinitionRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["type_name"].is_null()){
            Serializer::read(json_context["type_name"], instance.m_type_name);
        }
        if(!json_context["component"].is_null()){
            Serializer::read(json_context["component"], instance.m_component);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::ObjectDefinitionRes& instance){
        Json::object  ret_context;
        
        Json::array m_components_json;
        for (auto& item : instance.m_components){
            m_components_json.emplace_back(Serializer::write(item));
        }
        ret_context.insert_or_assign("components",m_components_json);
        
        return  Json(ret_context);
    }
    template<>
    VKernel::ObjectDefinitionRes& Serializer::read(const Json& json_context, VKernel::ObjectDefinitionRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["components"].is_null()){
            assert(json_context["components"].is_array());
            Json::array array_m_components = json_context["components"].array_items();
            instance.m_components.resize(array_m_components.size());
            for (size_t index=0; index < array_m_components.size();++index){
                Serializer::read(array_m_components[index], instance.m_components[index]);
            }
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::ObjectInstanceRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("name", Serializer::write(instance.m_name));
        ret_context.insert_or_assign("definition", Serializer::write(instance.m_definition));
        Json::array m_instanced_components_json;
        for (auto& item : instance.m_instanced_components){
            m_instanced_components_json.emplace_back(Serializer::write(item));
        }
        ret_context.insert_or_assign("instanced_components",m_instanced_components_json);
        
        return  Json(ret_context);
    }
    template<>
    VKernel::ObjectInstanceRes& Serializer::read(const Json& json_context, VKernel::ObjectInstanceRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["name"].is_null()){
            Serializer::read(json_context["name"], instance.m_name);
        }
        if(!json_context["definition"].is_null()){
            Serializer::read(json_context["definition"], instance.m_definition);
        }
        if(!json_context["instanced_components"].is_null()){
            assert(json_context["instanced_components"].is_array());
            Json::array array_m_instanced_components = json_context["instanced_components"].array_items();
            instance.m_instanced_components.resize(array_m_instanced_components.size());
            for (size_t index=0; index < array_m_instanced_components.size();++index){
                Serializer::read(array_m_instanced_components[index], instance.m_instanced_components[index]);
            }
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::Vector2& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("x", Serializer::write(instance.x));
        ret_context.insert_or_assign("y", Serializer::write(instance.y));
        return  Json(ret_context);
    }
    template<>
    VKernel::Vector2& Serializer::read(const Json& json_context, VKernel::Vector2& instance){
        assert(json_context.is_object());
        
        if(!json_context["x"].is_null()){
            Serializer::read(json_context["x"], instance.x);
        }
        if(!json_context["y"].is_null()){
            Serializer::read(json_context["y"], instance.y);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::Matrix4x4_& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("v0", Serializer::write(instance.v0));
        ret_context.insert_or_assign("v1", Serializer::write(instance.v1));
        ret_context.insert_or_assign("v2", Serializer::write(instance.v2));
        ret_context.insert_or_assign("v3", Serializer::write(instance.v3));
        ret_context.insert_or_assign("v4", Serializer::write(instance.v4));
        ret_context.insert_or_assign("v5", Serializer::write(instance.v5));
        ret_context.insert_or_assign("v6", Serializer::write(instance.v6));
        ret_context.insert_or_assign("v7", Serializer::write(instance.v7));
        ret_context.insert_or_assign("v8", Serializer::write(instance.v8));
        ret_context.insert_or_assign("v9", Serializer::write(instance.v9));
        ret_context.insert_or_assign("v10", Serializer::write(instance.v10));
        ret_context.insert_or_assign("v11", Serializer::write(instance.v11));
        ret_context.insert_or_assign("v12", Serializer::write(instance.v12));
        ret_context.insert_or_assign("v13", Serializer::write(instance.v13));
        ret_context.insert_or_assign("v14", Serializer::write(instance.v14));
        ret_context.insert_or_assign("v15", Serializer::write(instance.v15));
        return  Json(ret_context);
    }
    template<>
    VKernel::Matrix4x4_& Serializer::read(const Json& json_context, VKernel::Matrix4x4_& instance){
        assert(json_context.is_object());
        
        if(!json_context["v0"].is_null()){
            Serializer::read(json_context["v0"], instance.v0);
        }
        if(!json_context["v1"].is_null()){
            Serializer::read(json_context["v1"], instance.v1);
        }
        if(!json_context["v2"].is_null()){
            Serializer::read(json_context["v2"], instance.v2);
        }
        if(!json_context["v3"].is_null()){
            Serializer::read(json_context["v3"], instance.v3);
        }
        if(!json_context["v4"].is_null()){
            Serializer::read(json_context["v4"], instance.v4);
        }
        if(!json_context["v5"].is_null()){
            Serializer::read(json_context["v5"], instance.v5);
        }
        if(!json_context["v6"].is_null()){
            Serializer::read(json_context["v6"], instance.v6);
        }
        if(!json_context["v7"].is_null()){
            Serializer::read(json_context["v7"], instance.v7);
        }
        if(!json_context["v8"].is_null()){
            Serializer::read(json_context["v8"], instance.v8);
        }
        if(!json_context["v9"].is_null()){
            Serializer::read(json_context["v9"], instance.v9);
        }
        if(!json_context["v10"].is_null()){
            Serializer::read(json_context["v10"], instance.v10);
        }
        if(!json_context["v11"].is_null()){
            Serializer::read(json_context["v11"], instance.v11);
        }
        if(!json_context["v12"].is_null()){
            Serializer::read(json_context["v12"], instance.v12);
        }
        if(!json_context["v13"].is_null()){
            Serializer::read(json_context["v13"], instance.v13);
        }
        if(!json_context["v14"].is_null()){
            Serializer::read(json_context["v14"], instance.v14);
        }
        if(!json_context["v15"].is_null()){
            Serializer::read(json_context["v15"], instance.v15);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const Games::AutoMotorComponent& instance){
        Json::object  ret_context;
        auto&&  json_context_0 = Serializer::write(*(VKernel::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        
        return  Json(ret_context);
    }
    template<>
    Games::AutoMotorComponent& Serializer::read(const Json& json_context, Games::AutoMotorComponent& instance){
        assert(json_context.is_object());
        Serializer::read(json_context,*(VKernel::Component*)&instance);
        
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::Component& instance){
        Json::object  ret_context;
        
        
        return  Json(ret_context);
    }
    template<>
    VKernel::Component& Serializer::read(const Json& json_context, VKernel::Component& instance){
        assert(json_context.is_object());
        
        
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::GameObjectMeshDesc& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("mesh_file", Serializer::write(instance.m_mesh_file));
        return  Json(ret_context);
    }
    template<>
    VKernel::GameObjectMeshDesc& Serializer::read(const Json& json_context, VKernel::GameObjectMeshDesc& instance){
        assert(json_context.is_object());
        
        if(!json_context["mesh_file"].is_null()){
            Serializer::read(json_context["mesh_file"], instance.m_mesh_file);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::GameObjectMaterialDesc& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("base_color_texture_file", Serializer::write(instance.m_base_color_texture_file));
        ret_context.insert_or_assign("normal_texture_file", Serializer::write(instance.m_normal_texture_file));
        ret_context.insert_or_assign("with_texture", Serializer::write(instance.m_with_texture));
        return  Json(ret_context);
    }
    template<>
    VKernel::GameObjectMaterialDesc& Serializer::read(const Json& json_context, VKernel::GameObjectMaterialDesc& instance){
        assert(json_context.is_object());
        
        if(!json_context["base_color_texture_file"].is_null()){
            Serializer::read(json_context["base_color_texture_file"], instance.m_base_color_texture_file);
        }
        if(!json_context["normal_texture_file"].is_null()){
            Serializer::read(json_context["normal_texture_file"], instance.m_normal_texture_file);
        }
        if(!json_context["with_texture"].is_null()){
            Serializer::read(json_context["with_texture"], instance.m_with_texture);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::GameObjectTransformDesc& instance){
        Json::object  ret_context;
        
        
        return  Json(ret_context);
    }
    template<>
    VKernel::GameObjectTransformDesc& Serializer::read(const Json& json_context, VKernel::GameObjectTransformDesc& instance){
        assert(json_context.is_object());
        
        
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::GameObjectPartDesc& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("mesh_desc", Serializer::write(instance.m_mesh_desc));
        ret_context.insert_or_assign("material_desc", Serializer::write(instance.m_material_desc));
        ret_context.insert_or_assign("transform_desc", Serializer::write(instance.m_transform_desc));
        ret_context.insert_or_assign("color", Serializer::write(instance.m_color));
        ret_context.insert_or_assign("apply_lighting", Serializer::write(instance.m_apply_lighting));
        ret_context.insert_or_assign("apply_texture", Serializer::write(instance.m_apply_texture));
        ret_context.insert_or_assign("name", Serializer::write(instance.m_name));
        ret_context.insert_or_assign("introduction", Serializer::write(instance.m_introduction));
        ret_context.insert_or_assign("type", Serializer::write(instance.m_type));
        return  Json(ret_context);
    }
    template<>
    VKernel::GameObjectPartDesc& Serializer::read(const Json& json_context, VKernel::GameObjectPartDesc& instance){
        assert(json_context.is_object());
        
        if(!json_context["mesh_desc"].is_null()){
            Serializer::read(json_context["mesh_desc"], instance.m_mesh_desc);
        }
        if(!json_context["material_desc"].is_null()){
            Serializer::read(json_context["material_desc"], instance.m_material_desc);
        }
        if(!json_context["transform_desc"].is_null()){
            Serializer::read(json_context["transform_desc"], instance.m_transform_desc);
        }
        if(!json_context["color"].is_null()){
            Serializer::read(json_context["color"], instance.m_color);
        }
        if(!json_context["apply_lighting"].is_null()){
            Serializer::read(json_context["apply_lighting"], instance.m_apply_lighting);
        }
        if(!json_context["apply_texture"].is_null()){
            Serializer::read(json_context["apply_texture"], instance.m_apply_texture);
        }
        if(!json_context["name"].is_null()){
            Serializer::read(json_context["name"], instance.m_name);
        }
        if(!json_context["introduction"].is_null()){
            Serializer::read(json_context["introduction"], instance.m_introduction);
        }
        if(!json_context["type"].is_null()){
            Serializer::read(json_context["type"], instance.m_type);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const Games::CameraComponent& instance){
        Json::object  ret_context;
        auto&&  json_context_0 = Serializer::write(*(VKernel::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("camera_res", Serializer::write(instance.m_camera_res));
        return  Json(ret_context);
    }
    template<>
    Games::CameraComponent& Serializer::read(const Json& json_context, Games::CameraComponent& instance){
        assert(json_context.is_object());
        Serializer::read(json_context,*(VKernel::Component*)&instance);
        if(!json_context["camera_res"].is_null()){
            Serializer::read(json_context["camera_res"], instance.m_camera_res);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::MotorComponentRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("move_speed", Serializer::write(instance.m_move_speed));
        ret_context.insert_or_assign("jump_height", Serializer::write(instance.m_jump_height));
        ret_context.insert_or_assign("max_move_speed_ratio", Serializer::write(instance.m_max_move_speed_ratio));
        ret_context.insert_or_assign("max_sprint_speed_ratio", Serializer::write(instance.m_max_sprint_speed_ratio));
        ret_context.insert_or_assign("move_acceleration", Serializer::write(instance.m_move_acceleration));
        ret_context.insert_or_assign("sprint_acceleration", Serializer::write(instance.m_sprint_acceleration));
        return  Json(ret_context);
    }
    template<>
    VKernel::MotorComponentRes& Serializer::read(const Json& json_context, VKernel::MotorComponentRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["move_speed"].is_null()){
            Serializer::read(json_context["move_speed"], instance.m_move_speed);
        }
        if(!json_context["jump_height"].is_null()){
            Serializer::read(json_context["jump_height"], instance.m_jump_height);
        }
        if(!json_context["max_move_speed_ratio"].is_null()){
            Serializer::read(json_context["max_move_speed_ratio"], instance.m_max_move_speed_ratio);
        }
        if(!json_context["max_sprint_speed_ratio"].is_null()){
            Serializer::read(json_context["max_sprint_speed_ratio"], instance.m_max_sprint_speed_ratio);
        }
        if(!json_context["move_acceleration"].is_null()){
            Serializer::read(json_context["move_acceleration"], instance.m_move_acceleration);
        }
        if(!json_context["sprint_acceleration"].is_null()){
            Serializer::read(json_context["sprint_acceleration"], instance.m_sprint_acceleration);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const Games::MotorComponent& instance){
        Json::object  ret_context;
        auto&&  json_context_0 = Serializer::write(*(VKernel::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("motor_res", Serializer::write(instance.m_motor_res));
        return  Json(ret_context);
    }
    template<>
    Games::MotorComponent& Serializer::read(const Json& json_context, Games::MotorComponent& instance){
        assert(json_context.is_object());
        Serializer::read(json_context,*(VKernel::Component*)&instance);
        if(!json_context["motor_res"].is_null()){
            Serializer::read(json_context["motor_res"], instance.m_motor_res);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::Color& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("r", Serializer::write(instance.r));
        ret_context.insert_or_assign("g", Serializer::write(instance.g));
        ret_context.insert_or_assign("b", Serializer::write(instance.b));
        return  Json(ret_context);
    }
    template<>
    VKernel::Color& Serializer::read(const Json& json_context, VKernel::Color& instance){
        assert(json_context.is_object());
        
        if(!json_context["r"].is_null()){
            Serializer::read(json_context["r"], instance.r);
        }
        if(!json_context["g"].is_null()){
            Serializer::read(json_context["g"], instance.g);
        }
        if(!json_context["b"].is_null()){
            Serializer::read(json_context["b"], instance.b);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::AxisAlignedBox& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("center", Serializer::write(instance.m_center));
        ret_context.insert_or_assign("half_extent", Serializer::write(instance.m_half_extent));
        ret_context.insert_or_assign("min_corner", Serializer::write(instance.m_min_corner));
        ret_context.insert_or_assign("max_corner", Serializer::write(instance.m_max_corner));
        return  Json(ret_context);
    }
    template<>
    VKernel::AxisAlignedBox& Serializer::read(const Json& json_context, VKernel::AxisAlignedBox& instance){
        assert(json_context.is_object());
        
        if(!json_context["center"].is_null()){
            Serializer::read(json_context["center"], instance.m_center);
        }
        if(!json_context["half_extent"].is_null()){
            Serializer::read(json_context["half_extent"], instance.m_half_extent);
        }
        if(!json_context["min_corner"].is_null()){
            Serializer::read(json_context["min_corner"], instance.m_min_corner);
        }
        if(!json_context["max_corner"].is_null()){
            Serializer::read(json_context["max_corner"], instance.m_max_corner);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::SubMeshRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("obj_file_ref", Serializer::write(instance.m_obj_file_ref));
        ret_context.insert_or_assign("transform", Serializer::write(instance.m_transform));
        ret_context.insert_or_assign("material", Serializer::write(instance.m_material));
        return  Json(ret_context);
    }
    template<>
    VKernel::SubMeshRes& Serializer::read(const Json& json_context, VKernel::SubMeshRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["obj_file_ref"].is_null()){
            Serializer::read(json_context["obj_file_ref"], instance.m_obj_file_ref);
        }
        if(!json_context["transform"].is_null()){
            Serializer::read(json_context["transform"], instance.m_transform);
        }
        if(!json_context["material"].is_null()){
            Serializer::read(json_context["material"], instance.m_material);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::MeshComponentRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("color", Serializer::write(instance.m_color));
        ret_context.insert_or_assign("apply_lighting", Serializer::write(instance.m_apply_lighting));
        ret_context.insert_or_assign("apply_texture", Serializer::write(instance.m_apply_texture));
        ret_context.insert_or_assign("name", Serializer::write(instance.m_name));
        ret_context.insert_or_assign("introduction", Serializer::write(instance.m_introduction));
        ret_context.insert_or_assign("type", Serializer::write(instance.m_type));
        Json::array m_sub_meshes_json;
        for (auto& item : instance.m_sub_meshes){
            m_sub_meshes_json.emplace_back(Serializer::write(item));
        }
        ret_context.insert_or_assign("sub_meshes",m_sub_meshes_json);
        
        return  Json(ret_context);
    }
    template<>
    VKernel::MeshComponentRes& Serializer::read(const Json& json_context, VKernel::MeshComponentRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["color"].is_null()){
            Serializer::read(json_context["color"], instance.m_color);
        }
        if(!json_context["apply_lighting"].is_null()){
            Serializer::read(json_context["apply_lighting"], instance.m_apply_lighting);
        }
        if(!json_context["apply_texture"].is_null()){
            Serializer::read(json_context["apply_texture"], instance.m_apply_texture);
        }
        if(!json_context["name"].is_null()){
            Serializer::read(json_context["name"], instance.m_name);
        }
        if(!json_context["introduction"].is_null()){
            Serializer::read(json_context["introduction"], instance.m_introduction);
        }
        if(!json_context["type"].is_null()){
            Serializer::read(json_context["type"], instance.m_type);
        }
        if(!json_context["sub_meshes"].is_null()){
            assert(json_context["sub_meshes"].is_array());
            Json::array array_m_sub_meshes = json_context["sub_meshes"].array_items();
            instance.m_sub_meshes.resize(array_m_sub_meshes.size());
            for (size_t index=0; index < array_m_sub_meshes.size();++index){
                Serializer::read(array_m_sub_meshes[index], instance.m_sub_meshes[index]);
            }
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::MeshComponent& instance){
        Json::object  ret_context;
        auto&&  json_context_0 = Serializer::write(*(VKernel::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("mesh_res", Serializer::write(instance.m_mesh_res));
        return  Json(ret_context);
    }
    template<>
    VKernel::MeshComponent& Serializer::read(const Json& json_context, VKernel::MeshComponent& instance){
        assert(json_context.is_object());
        Serializer::read(json_context,*(VKernel::Component*)&instance);
        if(!json_context["mesh_res"].is_null()){
            Serializer::read(json_context["mesh_res"], instance.m_mesh_res);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::CameraPose& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("position", Serializer::write(instance.m_position));
        ret_context.insert_or_assign("target", Serializer::write(instance.m_target));
        ret_context.insert_or_assign("up", Serializer::write(instance.m_up));
        return  Json(ret_context);
    }
    template<>
    VKernel::CameraPose& Serializer::read(const Json& json_context, VKernel::CameraPose& instance){
        assert(json_context.is_object());
        
        if(!json_context["position"].is_null()){
            Serializer::read(json_context["position"], instance.m_position);
        }
        if(!json_context["target"].is_null()){
            Serializer::read(json_context["target"], instance.m_target);
        }
        if(!json_context["up"].is_null()){
            Serializer::read(json_context["up"], instance.m_up);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::CameraConfig& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("pose", Serializer::write(instance.m_pose));
        ret_context.insert_or_assign("aspect", Serializer::write(instance.m_aspect));
        ret_context.insert_or_assign("z_far", Serializer::write(instance.m_z_far));
        ret_context.insert_or_assign("z_near", Serializer::write(instance.m_z_near));
        return  Json(ret_context);
    }
    template<>
    VKernel::CameraConfig& Serializer::read(const Json& json_context, VKernel::CameraConfig& instance){
        assert(json_context.is_object());
        
        if(!json_context["pose"].is_null()){
            Serializer::read(json_context["pose"], instance.m_pose);
        }
        if(!json_context["aspect"].is_null()){
            Serializer::read(json_context["aspect"], instance.m_aspect);
        }
        if(!json_context["z_far"].is_null()){
            Serializer::read(json_context["z_far"], instance.m_z_far);
        }
        if(!json_context["z_near"].is_null()){
            Serializer::read(json_context["z_near"], instance.m_z_near);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::TransformComponent& instance){
        Json::object  ret_context;
        auto&&  json_context_0 = Serializer::write(*(VKernel::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("transform", Serializer::write(instance.m_transform));
        return  Json(ret_context);
    }
    template<>
    VKernel::TransformComponent& Serializer::read(const Json& json_context, VKernel::TransformComponent& instance){
        assert(json_context.is_object());
        Serializer::read(json_context,*(VKernel::Component*)&instance);
        if(!json_context["transform"].is_null()){
            Serializer::read(json_context["transform"], instance.m_transform);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::MaterialRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("base_colour_texture_file", Serializer::write(instance.m_base_colour_texture_file));
        ret_context.insert_or_assign("normal_texture_file", Serializer::write(instance.m_normal_texture_file));
        return  Json(ret_context);
    }
    template<>
    VKernel::MaterialRes& Serializer::read(const Json& json_context, VKernel::MaterialRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["base_colour_texture_file"].is_null()){
            Serializer::read(json_context["base_colour_texture_file"], instance.m_base_colour_texture_file);
        }
        if(!json_context["normal_texture_file"].is_null()){
            Serializer::read(json_context["normal_texture_file"], instance.m_normal_texture_file);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::SkyBoxIrradianceMap& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("negative_x_map", Serializer::write(instance.m_negative_x_map));
        ret_context.insert_or_assign("positive_x_map", Serializer::write(instance.m_positive_x_map));
        ret_context.insert_or_assign("negative_y_map", Serializer::write(instance.m_negative_y_map));
        ret_context.insert_or_assign("positive_y_map", Serializer::write(instance.m_positive_y_map));
        ret_context.insert_or_assign("negative_z_map", Serializer::write(instance.m_negative_z_map));
        ret_context.insert_or_assign("positive_z_map", Serializer::write(instance.m_positive_z_map));
        return  Json(ret_context);
    }
    template<>
    VKernel::SkyBoxIrradianceMap& Serializer::read(const Json& json_context, VKernel::SkyBoxIrradianceMap& instance){
        assert(json_context.is_object());
        
        if(!json_context["negative_x_map"].is_null()){
            Serializer::read(json_context["negative_x_map"], instance.m_negative_x_map);
        }
        if(!json_context["positive_x_map"].is_null()){
            Serializer::read(json_context["positive_x_map"], instance.m_positive_x_map);
        }
        if(!json_context["negative_y_map"].is_null()){
            Serializer::read(json_context["negative_y_map"], instance.m_negative_y_map);
        }
        if(!json_context["positive_y_map"].is_null()){
            Serializer::read(json_context["positive_y_map"], instance.m_positive_y_map);
        }
        if(!json_context["negative_z_map"].is_null()){
            Serializer::read(json_context["negative_z_map"], instance.m_negative_z_map);
        }
        if(!json_context["positive_z_map"].is_null()){
            Serializer::read(json_context["positive_z_map"], instance.m_positive_z_map);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::SkyBoxSpecularMap& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("negative_x_map", Serializer::write(instance.m_negative_x_map));
        ret_context.insert_or_assign("positive_x_map", Serializer::write(instance.m_positive_x_map));
        ret_context.insert_or_assign("negative_y_map", Serializer::write(instance.m_negative_y_map));
        ret_context.insert_or_assign("positive_y_map", Serializer::write(instance.m_positive_y_map));
        ret_context.insert_or_assign("negative_z_map", Serializer::write(instance.m_negative_z_map));
        ret_context.insert_or_assign("positive_z_map", Serializer::write(instance.m_positive_z_map));
        return  Json(ret_context);
    }
    template<>
    VKernel::SkyBoxSpecularMap& Serializer::read(const Json& json_context, VKernel::SkyBoxSpecularMap& instance){
        assert(json_context.is_object());
        
        if(!json_context["negative_x_map"].is_null()){
            Serializer::read(json_context["negative_x_map"], instance.m_negative_x_map);
        }
        if(!json_context["positive_x_map"].is_null()){
            Serializer::read(json_context["positive_x_map"], instance.m_positive_x_map);
        }
        if(!json_context["negative_y_map"].is_null()){
            Serializer::read(json_context["negative_y_map"], instance.m_negative_y_map);
        }
        if(!json_context["positive_y_map"].is_null()){
            Serializer::read(json_context["positive_y_map"], instance.m_positive_y_map);
        }
        if(!json_context["negative_z_map"].is_null()){
            Serializer::read(json_context["negative_z_map"], instance.m_negative_z_map);
        }
        if(!json_context["positive_z_map"].is_null()){
            Serializer::read(json_context["positive_z_map"], instance.m_positive_z_map);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::DirectionalLight& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("direction", Serializer::write(instance.m_direction));
        ret_context.insert_or_assign("color", Serializer::write(instance.m_color));
        return  Json(ret_context);
    }
    template<>
    VKernel::DirectionalLight& Serializer::read(const Json& json_context, VKernel::DirectionalLight& instance){
        assert(json_context.is_object());
        
        if(!json_context["direction"].is_null()){
            Serializer::read(json_context["direction"], instance.m_direction);
        }
        if(!json_context["color"].is_null()){
            Serializer::read(json_context["color"], instance.m_color);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::PointLights& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("position", Serializer::write(instance.m_position));
        ret_context.insert_or_assign("flux", Serializer::write(instance.m_flux));
        return  Json(ret_context);
    }
    template<>
    VKernel::PointLights& Serializer::read(const Json& json_context, VKernel::PointLights& instance){
        assert(json_context.is_object());
        
        if(!json_context["position"].is_null()){
            Serializer::read(json_context["position"], instance.m_position);
        }
        if(!json_context["flux"].is_null()){
            Serializer::read(json_context["flux"], instance.m_flux);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const VKernel::GlobalRenderingRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("skybox_irradiance_map", Serializer::write(instance.m_skybox_irradiance_map));
        ret_context.insert_or_assign("skybox_specular_map", Serializer::write(instance.m_skybox_specular_map));
        ret_context.insert_or_assign("brdf_map", Serializer::write(instance.m_brdf_map));
        ret_context.insert_or_assign("color_grading_map", Serializer::write(instance.m_color_grading_map));
        ret_context.insert_or_assign("ambient_light", Serializer::write(instance.m_ambient_light));
        ret_context.insert_or_assign("camera_config", Serializer::write(instance.m_camera_config));
        ret_context.insert_or_assign("directional_light", Serializer::write(instance.m_directional_light));
        Json::array m_point_lights_json;
        for (auto& item : instance.m_point_lights){
            m_point_lights_json.emplace_back(Serializer::write(item));
        }
        ret_context.insert_or_assign("point_lights",m_point_lights_json);
        
        return  Json(ret_context);
    }
    template<>
    VKernel::GlobalRenderingRes& Serializer::read(const Json& json_context, VKernel::GlobalRenderingRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["skybox_irradiance_map"].is_null()){
            Serializer::read(json_context["skybox_irradiance_map"], instance.m_skybox_irradiance_map);
        }
        if(!json_context["skybox_specular_map"].is_null()){
            Serializer::read(json_context["skybox_specular_map"], instance.m_skybox_specular_map);
        }
        if(!json_context["brdf_map"].is_null()){
            Serializer::read(json_context["brdf_map"], instance.m_brdf_map);
        }
        if(!json_context["color_grading_map"].is_null()){
            Serializer::read(json_context["color_grading_map"], instance.m_color_grading_map);
        }
        if(!json_context["ambient_light"].is_null()){
            Serializer::read(json_context["ambient_light"], instance.m_ambient_light);
        }
        if(!json_context["camera_config"].is_null()){
            Serializer::read(json_context["camera_config"], instance.m_camera_config);
        }
        if(!json_context["directional_light"].is_null()){
            Serializer::read(json_context["directional_light"], instance.m_directional_light);
        }
        if(!json_context["point_lights"].is_null()){
            assert(json_context["point_lights"].is_array());
            Json::array array_m_point_lights = json_context["point_lights"].array_items();
            instance.m_point_lights.resize(array_m_point_lights.size());
            for (size_t index=0; index < array_m_point_lights.size();++index){
                Serializer::read(array_m_point_lights[index], instance.m_point_lights[index]);
            }
        }
        return instance;
    }

}


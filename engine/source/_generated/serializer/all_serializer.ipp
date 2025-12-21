#pragma once
#include "_generated/serializer/quaternion.serializer.gen.h"
#include "_generated/serializer/transform.serializer.gen.h"
#include "_generated/serializer/vector3.serializer.gen.h"
#include "_generated/serializer/vector4.serializer.gen.h"
#include "_generated/serializer/axis_aligned.serializer.gen.h"
#include "_generated/serializer/matrix4.serializer.gen.h"
#include "_generated/serializer/vector2.serializer.gen.h"
#include "_generated/serializer/component.serializer.gen.h"
#include "_generated/serializer/camera_component.serializer.gen.h"
#include "_generated/serializer/camera_config.serializer.gen.h"
#include "_generated/serializer/global_rendering.serializer.gen.h"
namespace VKernel{
    template<>
    Json Serializer::write(const Quaternion& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("w", Serializer::write(instance.w));
        ret_context.insert_or_assign("x", Serializer::write(instance.x));
        ret_context.insert_or_assign("y", Serializer::write(instance.y));
        ret_context.insert_or_assign("z", Serializer::write(instance.z));
        return  Json(ret_context);
    }
    template<>
    Quaternion& Serializer::read(const Json& json_context, Quaternion& instance){
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
    Json Serializer::write(const Transform& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("position", Serializer::write(instance.m_position));
        ret_context.insert_or_assign("scale", Serializer::write(instance.m_scale));
        ret_context.insert_or_assign("rotation", Serializer::write(instance.m_rotation));
        return  Json(ret_context);
    }
    template<>
    Transform& Serializer::read(const Json& json_context, Transform& instance){
        assert(json_context.is_object());
        
        if(!json_context["position"].is_null()){
            Serializer::read(json_context["position"], instance.m_position);
        }
        if(!json_context["scale"].is_null()){
            Serializer::read(json_context["scale"], instance.m_scale);
        }
        if(!json_context["rotation"].is_null()){
            Serializer::read(json_context["rotation"], instance.m_rotation);
        }
        return instance;
    }
    template<>
    Json Serializer::write(const Vector3& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("x", Serializer::write(instance.x));
        ret_context.insert_or_assign("y", Serializer::write(instance.y));
        ret_context.insert_or_assign("z", Serializer::write(instance.z));
        return  Json(ret_context);
    }
    template<>
    Vector3& Serializer::read(const Json& json_context, Vector3& instance){
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
    Json Serializer::write(const Vector4& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("x", Serializer::write(instance.x));
        ret_context.insert_or_assign("y", Serializer::write(instance.y));
        ret_context.insert_or_assign("z", Serializer::write(instance.z));
        ret_context.insert_or_assign("w", Serializer::write(instance.w));
        return  Json(ret_context);
    }
    template<>
    Vector4& Serializer::read(const Json& json_context, Vector4& instance){
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
    Json Serializer::write(const AxisAlignedBox& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("center", Serializer::write(instance.m_center));
        ret_context.insert_or_assign("half_extent", Serializer::write(instance.m_half_extent));
        ret_context.insert_or_assign("min_corner", Serializer::write(instance.m_min_corner));
        ret_context.insert_or_assign("max_corner", Serializer::write(instance.m_max_corner));
        return  Json(ret_context);
    }
    template<>
    AxisAlignedBox& Serializer::read(const Json& json_context, AxisAlignedBox& instance){
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
    Json Serializer::write(const Matrix4x4_& instance){
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
    Matrix4x4_& Serializer::read(const Json& json_context, Matrix4x4_& instance){
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
    Json Serializer::write(const Vector2& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("x", Serializer::write(instance.x));
        ret_context.insert_or_assign("y", Serializer::write(instance.y));
        return  Json(ret_context);
    }
    template<>
    Vector2& Serializer::read(const Json& json_context, Vector2& instance){
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
    Json Serializer::write(const Component& instance){
        Json::object  ret_context;
        
        
        return  Json(ret_context);
    }
    template<>
    Component& Serializer::read(const Json& json_context, Component& instance){
        assert(json_context.is_object());
        
        
        return instance;
    }
    template<>
    Json Serializer::write(const CameraComponent& instance){
        Json::object  ret_context;
        auto&&  json_context_0 = Serializer::write(*(VKernel::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        
        return  Json(ret_context);
    }
    template<>
    CameraComponent& Serializer::read(const Json& json_context, CameraComponent& instance){
        assert(json_context.is_object());
        Serializer::read(json_context,*(VKernel::Component*)&instance);
        
        return instance;
    }
    template<>
    Json Serializer::write(const CameraPose& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("position", Serializer::write(instance.m_position));
        ret_context.insert_or_assign("target", Serializer::write(instance.m_target));
        ret_context.insert_or_assign("up", Serializer::write(instance.m_up));
        return  Json(ret_context);
    }
    template<>
    CameraPose& Serializer::read(const Json& json_context, CameraPose& instance){
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
    Json Serializer::write(const CameraConfig& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("pose", Serializer::write(instance.m_pose));
        ret_context.insert_or_assign("aspect", Serializer::write(instance.m_aspect));
        ret_context.insert_or_assign("z_far", Serializer::write(instance.m_z_far));
        ret_context.insert_or_assign("z_near", Serializer::write(instance.m_z_near));
        return  Json(ret_context);
    }
    template<>
    CameraConfig& Serializer::read(const Json& json_context, CameraConfig& instance){
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
    Json Serializer::write(const GlobalRenderingRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("camera_config", Serializer::write(instance.m_camera_config));
        return  Json(ret_context);
    }
    template<>
    GlobalRenderingRes& Serializer::read(const Json& json_context, GlobalRenderingRes& instance){
        assert(json_context.is_object());
        
        if(!json_context["camera_config"].is_null()){
            Serializer::read(json_context["camera_config"], instance.m_camera_config);
        }
        return instance;
    }

}


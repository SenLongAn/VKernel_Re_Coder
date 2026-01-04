#pragma once
#include "_generated/serializer/quaternion.serializer.gen.h"
#include "_generated/serializer/transform.serializer.gen.h"
#include "_generated/serializer/vector3.serializer.gen.h"
#include "_generated/serializer/matrix4.serializer.gen.h"
#include "_generated/serializer/vector2.serializer.gen.h"
#include "_generated/serializer/color.serializer.gen.h"
#include "_generated/serializer/vector4.serializer.gen.h"
#include "_generated/serializer/axis_aligned.serializer.gen.h"
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
    Json Serializer::write(const Color& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("r", Serializer::write(instance.r));
        ret_context.insert_or_assign("g", Serializer::write(instance.g));
        ret_context.insert_or_assign("b", Serializer::write(instance.b));
        return  Json(ret_context);
    }
    template<>
    Color& Serializer::read(const Json& json_context, Color& instance){
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
    Json Serializer::write(const SkyBoxIrradianceMap& instance){
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
    SkyBoxIrradianceMap& Serializer::read(const Json& json_context, SkyBoxIrradianceMap& instance){
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
    Json Serializer::write(const SkyBoxSpecularMap& instance){
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
    SkyBoxSpecularMap& Serializer::read(const Json& json_context, SkyBoxSpecularMap& instance){
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
    Json Serializer::write(const DirectionalLight& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("direction", Serializer::write(instance.m_direction));
        ret_context.insert_or_assign("color", Serializer::write(instance.m_color));
        return  Json(ret_context);
    }
    template<>
    DirectionalLight& Serializer::read(const Json& json_context, DirectionalLight& instance){
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
    Json Serializer::write(const PointLights& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("position", Serializer::write(instance.m_position));
        ret_context.insert_or_assign("flux", Serializer::write(instance.m_flux));
        return  Json(ret_context);
    }
    template<>
    PointLights& Serializer::read(const Json& json_context, PointLights& instance){
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
    Json Serializer::write(const GlobalRenderingRes& instance){
        Json::object  ret_context;
        
        ret_context.insert_or_assign("skybox_irradiance_map", Serializer::write(instance.m_skybox_irradiance_map));
        ret_context.insert_or_assign("skybox_specular_map", Serializer::write(instance.m_skybox_specular_map));
        ret_context.insert_or_assign("brdf_map", Serializer::write(instance.m_brdf_map));
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
    GlobalRenderingRes& Serializer::read(const Json& json_context, GlobalRenderingRes& instance){
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


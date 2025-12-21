#pragma once
#include "runtime/resource/res_type/data/camera_config.h"

namespace VKernel{
    class CameraPose;
    class CameraConfig;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeCameraPoseOperator{
    public:
        static const char* getClassName(){ return "CameraPose";}
        static void* constructorWithJson(const Json& json_context){
            CameraPose* ret_instance= new CameraPose;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(CameraPose*)instance);
        }
        // base class
        static int getCameraPoseBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_position(){ return "m_position";}
        static const char* getFieldTypeName_m_position(){ return "Vector3";}
        static void set_m_position(void* instance, void* field_value){ static_cast<CameraPose*>(instance)->m_position = *static_cast<Vector3*>(field_value);}
        static void* get_m_position(void* instance){ return static_cast<void*>(&(static_cast<CameraPose*>(instance)->m_position));}
        static bool isArray_m_position(){ return false; }
        static const char* getFieldName_m_target(){ return "m_target";}
        static const char* getFieldTypeName_m_target(){ return "Vector3";}
        static void set_m_target(void* instance, void* field_value){ static_cast<CameraPose*>(instance)->m_target = *static_cast<Vector3*>(field_value);}
        static void* get_m_target(void* instance){ return static_cast<void*>(&(static_cast<CameraPose*>(instance)->m_target));}
        static bool isArray_m_target(){ return false; }
        static const char* getFieldName_m_up(){ return "m_up";}
        static const char* getFieldTypeName_m_up(){ return "Vector3";}
        static void set_m_up(void* instance, void* field_value){ static_cast<CameraPose*>(instance)->m_up = *static_cast<Vector3*>(field_value);}
        static void* get_m_up(void* instance){ return static_cast<void*>(&(static_cast<CameraPose*>(instance)->m_up));}
        static bool isArray_m_up(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_CameraPose(){
        FieldFunctionTuple* field_function_tuple_m_position=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::set_m_position,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::get_m_position,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getFieldName_m_position,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getFieldTypeName_m_position,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::isArray_m_position);
        REGISTER_FIELD_TO_MAP("CameraPose", field_function_tuple_m_position);
        FieldFunctionTuple* field_function_tuple_m_target=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::set_m_target,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::get_m_target,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getFieldName_m_target,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getFieldTypeName_m_target,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::isArray_m_target);
        REGISTER_FIELD_TO_MAP("CameraPose", field_function_tuple_m_target);
        FieldFunctionTuple* field_function_tuple_m_up=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::set_m_up,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::get_m_up,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getFieldName_m_up,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getFieldTypeName_m_up,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::isArray_m_up);
        REGISTER_FIELD_TO_MAP("CameraPose", field_function_tuple_m_up);

        
        
        
        ClassFunctionTuple* class_function_tuple_CameraPose=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::getCameraPoseBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeCameraPoseOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("CameraPose", class_function_tuple_CameraPose);
    }namespace TypeFieldReflectionOparator{
    class TypeCameraConfigOperator{
    public:
        static const char* getClassName(){ return "CameraConfig";}
        static void* constructorWithJson(const Json& json_context){
            CameraConfig* ret_instance= new CameraConfig;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(CameraConfig*)instance);
        }
        // base class
        static int getCameraConfigBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_pose(){ return "m_pose";}
        static const char* getFieldTypeName_m_pose(){ return "CameraPose";}
        static void set_m_pose(void* instance, void* field_value){ static_cast<CameraConfig*>(instance)->m_pose = *static_cast<CameraPose*>(field_value);}
        static void* get_m_pose(void* instance){ return static_cast<void*>(&(static_cast<CameraConfig*>(instance)->m_pose));}
        static bool isArray_m_pose(){ return false; }
        static const char* getFieldName_m_aspect(){ return "m_aspect";}
        static const char* getFieldTypeName_m_aspect(){ return "Vector2";}
        static void set_m_aspect(void* instance, void* field_value){ static_cast<CameraConfig*>(instance)->m_aspect = *static_cast<Vector2*>(field_value);}
        static void* get_m_aspect(void* instance){ return static_cast<void*>(&(static_cast<CameraConfig*>(instance)->m_aspect));}
        static bool isArray_m_aspect(){ return false; }
        static const char* getFieldName_m_z_far(){ return "m_z_far";}
        static const char* getFieldTypeName_m_z_far(){ return "float";}
        static void set_m_z_far(void* instance, void* field_value){ static_cast<CameraConfig*>(instance)->m_z_far = *static_cast<float*>(field_value);}
        static void* get_m_z_far(void* instance){ return static_cast<void*>(&(static_cast<CameraConfig*>(instance)->m_z_far));}
        static bool isArray_m_z_far(){ return false; }
        static const char* getFieldName_m_z_near(){ return "m_z_near";}
        static const char* getFieldTypeName_m_z_near(){ return "float";}
        static void set_m_z_near(void* instance, void* field_value){ static_cast<CameraConfig*>(instance)->m_z_near = *static_cast<float*>(field_value);}
        static void* get_m_z_near(void* instance){ return static_cast<void*>(&(static_cast<CameraConfig*>(instance)->m_z_near));}
        static bool isArray_m_z_near(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_CameraConfig(){
        FieldFunctionTuple* field_function_tuple_m_pose=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::set_m_pose,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::get_m_pose,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getFieldName_m_pose,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getFieldTypeName_m_pose,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::isArray_m_pose);
        REGISTER_FIELD_TO_MAP("CameraConfig", field_function_tuple_m_pose);
        FieldFunctionTuple* field_function_tuple_m_aspect=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::set_m_aspect,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::get_m_aspect,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getFieldName_m_aspect,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getFieldTypeName_m_aspect,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::isArray_m_aspect);
        REGISTER_FIELD_TO_MAP("CameraConfig", field_function_tuple_m_aspect);
        FieldFunctionTuple* field_function_tuple_m_z_far=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::set_m_z_far,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::get_m_z_far,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getFieldName_m_z_far,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getFieldTypeName_m_z_far,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::isArray_m_z_far);
        REGISTER_FIELD_TO_MAP("CameraConfig", field_function_tuple_m_z_far);
        FieldFunctionTuple* field_function_tuple_m_z_near=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::set_m_z_near,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::get_m_z_near,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getFieldName_m_z_near,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getFieldTypeName_m_z_near,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::isArray_m_z_near);
        REGISTER_FIELD_TO_MAP("CameraConfig", field_function_tuple_m_z_near);

        
        
        
        ClassFunctionTuple* class_function_tuple_CameraConfig=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::getCameraConfigBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeCameraConfigOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("CameraConfig", class_function_tuple_CameraConfig);
    }
namespace TypeWrappersRegister{
    void CameraConfig()
    {
        TypeWrapperRegister_CameraPose();
    TypeWrapperRegister_CameraConfig();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


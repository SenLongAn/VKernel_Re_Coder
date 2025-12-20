#pragma once
#include "runtime/function/framework/component/camera/camera_component.h"

namespace VKernel{
    class CameraComponent;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeCameraComponentOperator{
    public:
        static const char* getClassName(){ return "CameraComponent";}
        static void* constructorWithJson(const Json& json_context){
            CameraComponent* ret_instance= new CameraComponent;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(CameraComponent*)instance);
        }
        // base class
        static int getCameraComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 1;
            out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
               out_list[i] = TypeMetaDef(VKernel::Component,static_cast<CameraComponent*>(instance));
            }
            return count;
        }
        // fields
        static const char* getFieldName_temp(){ return "temp";}
        static const char* getFieldTypeName_temp(){ return "int";}
        static void set_temp(void* instance, void* field_value){ static_cast<CameraComponent*>(instance)->temp = *static_cast<int*>(field_value);}
        static void* get_temp(void* instance){ return static_cast<void*>(&(static_cast<CameraComponent*>(instance)->temp));}
        static bool isArray_temp(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_CameraComponent(){
        FieldFunctionTuple* field_function_tuple_temp=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::set_temp,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::get_temp,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getFieldName_temp,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getFieldTypeName_temp,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::isArray_temp);
        REGISTER_FIELD_TO_MAP("CameraComponent", field_function_tuple_temp);

        
        
        
        ClassFunctionTuple* class_function_tuple_CameraComponent=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getCameraComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("CameraComponent", class_function_tuple_CameraComponent);
    }
namespace TypeWrappersRegister{
    void CameraComponent()
    {
        TypeWrapperRegister_CameraComponent();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


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
        

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_CameraComponent(){
        

        
        
        
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


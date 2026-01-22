#pragma once
#include "runtime/function/framework/component/motor/motor_component.h"

namespace VKernel{
    class MotorComponent;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeMotorComponentOperator{
    public:
        static const char* getClassName(){ return "MotorComponent";}
        static void* constructorWithJson(const Json& json_context){
            MotorComponent* ret_instance= new MotorComponent;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(MotorComponent*)instance);
        }
        // base class
        static int getMotorComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 1;
            out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
               out_list[i] = TypeMetaDef(VKernel::Component,static_cast<MotorComponent*>(instance));
            }
            return count;
        }
        // fields
        

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_MotorComponent(){
        

        
        
        
        ClassFunctionTuple* class_function_tuple_MotorComponent=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeMotorComponentOperator::getMotorComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeMotorComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeMotorComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("MotorComponent", class_function_tuple_MotorComponent);
    }
namespace TypeWrappersRegister{
    void MotorComponent()
    {
        TypeWrapperRegister_MotorComponent();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


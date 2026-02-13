#pragma once
#include "runtime/Games/the_celestial_console/component/motor/auto_motor_component.h"

namespace VKernel{
    class Games::AutoMotorComponent;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeAutoMotorComponentOperator{
    public:
        static const char* getClassName(){ return "AutoMotorComponent";}
        static void* constructorWithJson(const Json& json_context){
            Games::AutoMotorComponent* ret_instance= new Games::AutoMotorComponent;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(Games::AutoMotorComponent*)instance);
        }
        // base class
        static int getAutoMotorComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 1;
            out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
               out_list[i] = TypeMetaDef(VKernel::Component,static_cast<Games::AutoMotorComponent*>(instance));
            }
            return count;
        }
        // fields
        

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_AutoMotorComponent(){
        

        
        
        
        ClassFunctionTuple* class_function_tuple_AutoMotorComponent=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeAutoMotorComponentOperator::getAutoMotorComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeAutoMotorComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeAutoMotorComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("AutoMotorComponent", class_function_tuple_AutoMotorComponent);
    }
namespace TypeWrappersRegister{
    void AutoMotorComponent()
    {
        TypeWrapperRegister_AutoMotorComponent();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


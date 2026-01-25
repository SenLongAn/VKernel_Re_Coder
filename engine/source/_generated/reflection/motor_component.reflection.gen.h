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
        static const char* getFieldName_m_motor_res(){ return "m_motor_res";}
        static const char* getFieldTypeName_m_motor_res(){ return "MotorComponentRes";}
        static void set_m_motor_res(void* instance, void* field_value){ static_cast<MotorComponent*>(instance)->m_motor_res = *static_cast<MotorComponentRes*>(field_value);}
        static void* get_m_motor_res(void* instance){ return static_cast<void*>(&(static_cast<MotorComponent*>(instance)->m_motor_res));}
        static bool isArray_m_motor_res(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_MotorComponent(){
        FieldFunctionTuple* field_function_tuple_m_motor_res=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMotorComponentOperator::set_m_motor_res,
            &TypeFieldReflectionOparator::TypeMotorComponentOperator::get_m_motor_res,
            &TypeFieldReflectionOparator::TypeMotorComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMotorComponentOperator::getFieldName_m_motor_res,
            &TypeFieldReflectionOparator::TypeMotorComponentOperator::getFieldTypeName_m_motor_res,
            &TypeFieldReflectionOparator::TypeMotorComponentOperator::isArray_m_motor_res);
        REGISTER_FIELD_TO_MAP("MotorComponent", field_function_tuple_m_motor_res);

        
        
        
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


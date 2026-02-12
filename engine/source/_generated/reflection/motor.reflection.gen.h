#pragma once
#include "runtime/Games/the_celestial_console/component/motor/motor.h"

namespace VKernel{
    class VKernel::MotorComponentRes;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeMotorComponentResOperator{
    public:
        static const char* getClassName(){ return "MotorComponentRes";}
        static void* constructorWithJson(const Json& json_context){
            VKernel::MotorComponentRes* ret_instance= new VKernel::MotorComponentRes;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(VKernel::MotorComponentRes*)instance);
        }
        // base class
        static int getMotorComponentResBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_move_speed(){ return "m_move_speed";}
        static const char* getFieldTypeName_m_move_speed(){ return "float";}
        static void set_m_move_speed(void* instance, void* field_value){ static_cast<VKernel::MotorComponentRes*>(instance)->m_move_speed = *static_cast<float*>(field_value);}
        static void* get_m_move_speed(void* instance){ return static_cast<void*>(&(static_cast<VKernel::MotorComponentRes*>(instance)->m_move_speed));}
        static bool isArray_m_move_speed(){ return false; }
        static const char* getFieldName_m_jump_height(){ return "m_jump_height";}
        static const char* getFieldTypeName_m_jump_height(){ return "float";}
        static void set_m_jump_height(void* instance, void* field_value){ static_cast<VKernel::MotorComponentRes*>(instance)->m_jump_height = *static_cast<float*>(field_value);}
        static void* get_m_jump_height(void* instance){ return static_cast<void*>(&(static_cast<VKernel::MotorComponentRes*>(instance)->m_jump_height));}
        static bool isArray_m_jump_height(){ return false; }
        static const char* getFieldName_m_max_move_speed_ratio(){ return "m_max_move_speed_ratio";}
        static const char* getFieldTypeName_m_max_move_speed_ratio(){ return "float";}
        static void set_m_max_move_speed_ratio(void* instance, void* field_value){ static_cast<VKernel::MotorComponentRes*>(instance)->m_max_move_speed_ratio = *static_cast<float*>(field_value);}
        static void* get_m_max_move_speed_ratio(void* instance){ return static_cast<void*>(&(static_cast<VKernel::MotorComponentRes*>(instance)->m_max_move_speed_ratio));}
        static bool isArray_m_max_move_speed_ratio(){ return false; }
        static const char* getFieldName_m_max_sprint_speed_ratio(){ return "m_max_sprint_speed_ratio";}
        static const char* getFieldTypeName_m_max_sprint_speed_ratio(){ return "float";}
        static void set_m_max_sprint_speed_ratio(void* instance, void* field_value){ static_cast<VKernel::MotorComponentRes*>(instance)->m_max_sprint_speed_ratio = *static_cast<float*>(field_value);}
        static void* get_m_max_sprint_speed_ratio(void* instance){ return static_cast<void*>(&(static_cast<VKernel::MotorComponentRes*>(instance)->m_max_sprint_speed_ratio));}
        static bool isArray_m_max_sprint_speed_ratio(){ return false; }
        static const char* getFieldName_m_move_acceleration(){ return "m_move_acceleration";}
        static const char* getFieldTypeName_m_move_acceleration(){ return "float";}
        static void set_m_move_acceleration(void* instance, void* field_value){ static_cast<VKernel::MotorComponentRes*>(instance)->m_move_acceleration = *static_cast<float*>(field_value);}
        static void* get_m_move_acceleration(void* instance){ return static_cast<void*>(&(static_cast<VKernel::MotorComponentRes*>(instance)->m_move_acceleration));}
        static bool isArray_m_move_acceleration(){ return false; }
        static const char* getFieldName_m_sprint_acceleration(){ return "m_sprint_acceleration";}
        static const char* getFieldTypeName_m_sprint_acceleration(){ return "float";}
        static void set_m_sprint_acceleration(void* instance, void* field_value){ static_cast<VKernel::MotorComponentRes*>(instance)->m_sprint_acceleration = *static_cast<float*>(field_value);}
        static void* get_m_sprint_acceleration(void* instance){ return static_cast<void*>(&(static_cast<VKernel::MotorComponentRes*>(instance)->m_sprint_acceleration));}
        static bool isArray_m_sprint_acceleration(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_MotorComponentRes(){
        FieldFunctionTuple* field_function_tuple_m_move_speed=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::set_m_move_speed,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::get_m_move_speed,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldName_m_move_speed,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldTypeName_m_move_speed,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::isArray_m_move_speed);
        REGISTER_FIELD_TO_MAP("MotorComponentRes", field_function_tuple_m_move_speed);
        FieldFunctionTuple* field_function_tuple_m_jump_height=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::set_m_jump_height,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::get_m_jump_height,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldName_m_jump_height,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldTypeName_m_jump_height,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::isArray_m_jump_height);
        REGISTER_FIELD_TO_MAP("MotorComponentRes", field_function_tuple_m_jump_height);
        FieldFunctionTuple* field_function_tuple_m_max_move_speed_ratio=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::set_m_max_move_speed_ratio,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::get_m_max_move_speed_ratio,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldName_m_max_move_speed_ratio,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldTypeName_m_max_move_speed_ratio,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::isArray_m_max_move_speed_ratio);
        REGISTER_FIELD_TO_MAP("MotorComponentRes", field_function_tuple_m_max_move_speed_ratio);
        FieldFunctionTuple* field_function_tuple_m_max_sprint_speed_ratio=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::set_m_max_sprint_speed_ratio,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::get_m_max_sprint_speed_ratio,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldName_m_max_sprint_speed_ratio,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldTypeName_m_max_sprint_speed_ratio,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::isArray_m_max_sprint_speed_ratio);
        REGISTER_FIELD_TO_MAP("MotorComponentRes", field_function_tuple_m_max_sprint_speed_ratio);
        FieldFunctionTuple* field_function_tuple_m_move_acceleration=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::set_m_move_acceleration,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::get_m_move_acceleration,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldName_m_move_acceleration,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldTypeName_m_move_acceleration,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::isArray_m_move_acceleration);
        REGISTER_FIELD_TO_MAP("MotorComponentRes", field_function_tuple_m_move_acceleration);
        FieldFunctionTuple* field_function_tuple_m_sprint_acceleration=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::set_m_sprint_acceleration,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::get_m_sprint_acceleration,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldName_m_sprint_acceleration,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getFieldTypeName_m_sprint_acceleration,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::isArray_m_sprint_acceleration);
        REGISTER_FIELD_TO_MAP("MotorComponentRes", field_function_tuple_m_sprint_acceleration);

        
        
        
        ClassFunctionTuple* class_function_tuple_MotorComponentRes=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::getMotorComponentResBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeMotorComponentResOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("MotorComponentRes", class_function_tuple_MotorComponentRes);
    }
namespace TypeWrappersRegister{
    void Motor()
    {
        TypeWrapperRegister_MotorComponentRes();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


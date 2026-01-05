#pragma once
#include "runtime/core/math/transform.h"

namespace VKernel{
    class Transform;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeTransformOperator{
    public:
        static const char* getClassName(){ return "Transform";}
        static void* constructorWithJson(const Json& json_context){
            Transform* ret_instance= new Transform;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(Transform*)instance);
        }
        // base class
        static int getTransformBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_position(){ return "m_position";}
        static const char* getFieldTypeName_m_position(){ return "Vector3";}
        static void set_m_position(void* instance, void* field_value){ static_cast<Transform*>(instance)->m_position = *static_cast<Vector3*>(field_value);}
        static void* get_m_position(void* instance){ return static_cast<void*>(&(static_cast<Transform*>(instance)->m_position));}
        static bool isArray_m_position(){ return false; }
        static const char* getFieldName_m_scale(){ return "m_scale";}
        static const char* getFieldTypeName_m_scale(){ return "Vector3";}
        static void set_m_scale(void* instance, void* field_value){ static_cast<Transform*>(instance)->m_scale = *static_cast<Vector3*>(field_value);}
        static void* get_m_scale(void* instance){ return static_cast<void*>(&(static_cast<Transform*>(instance)->m_scale));}
        static bool isArray_m_scale(){ return false; }
        static const char* getFieldName_m_euler_rotation(){ return "m_euler_rotation";}
        static const char* getFieldTypeName_m_euler_rotation(){ return "Vector3";}
        static void set_m_euler_rotation(void* instance, void* field_value){ static_cast<Transform*>(instance)->m_euler_rotation = *static_cast<Vector3*>(field_value);}
        static void* get_m_euler_rotation(void* instance){ return static_cast<void*>(&(static_cast<Transform*>(instance)->m_euler_rotation));}
        static bool isArray_m_euler_rotation(){ return false; }
        static const char* getFieldName_m_is_serialization(){ return "m_is_serialization";}
        static const char* getFieldTypeName_m_is_serialization(){ return "bool";}
        static void set_m_is_serialization(void* instance, void* field_value){ static_cast<Transform*>(instance)->m_is_serialization = *static_cast<bool*>(field_value);}
        static void* get_m_is_serialization(void* instance){ return static_cast<void*>(&(static_cast<Transform*>(instance)->m_is_serialization));}
        static bool isArray_m_is_serialization(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_Transform(){
        FieldFunctionTuple* field_function_tuple_m_position=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformOperator::set_m_position,
            &TypeFieldReflectionOparator::TypeTransformOperator::get_m_position,
            &TypeFieldReflectionOparator::TypeTransformOperator::getClassName,
            &TypeFieldReflectionOparator::TypeTransformOperator::getFieldName_m_position,
            &TypeFieldReflectionOparator::TypeTransformOperator::getFieldTypeName_m_position,
            &TypeFieldReflectionOparator::TypeTransformOperator::isArray_m_position);
        REGISTER_FIELD_TO_MAP("Transform", field_function_tuple_m_position);
        FieldFunctionTuple* field_function_tuple_m_scale=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformOperator::set_m_scale,
            &TypeFieldReflectionOparator::TypeTransformOperator::get_m_scale,
            &TypeFieldReflectionOparator::TypeTransformOperator::getClassName,
            &TypeFieldReflectionOparator::TypeTransformOperator::getFieldName_m_scale,
            &TypeFieldReflectionOparator::TypeTransformOperator::getFieldTypeName_m_scale,
            &TypeFieldReflectionOparator::TypeTransformOperator::isArray_m_scale);
        REGISTER_FIELD_TO_MAP("Transform", field_function_tuple_m_scale);
        FieldFunctionTuple* field_function_tuple_m_euler_rotation=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformOperator::set_m_euler_rotation,
            &TypeFieldReflectionOparator::TypeTransformOperator::get_m_euler_rotation,
            &TypeFieldReflectionOparator::TypeTransformOperator::getClassName,
            &TypeFieldReflectionOparator::TypeTransformOperator::getFieldName_m_euler_rotation,
            &TypeFieldReflectionOparator::TypeTransformOperator::getFieldTypeName_m_euler_rotation,
            &TypeFieldReflectionOparator::TypeTransformOperator::isArray_m_euler_rotation);
        REGISTER_FIELD_TO_MAP("Transform", field_function_tuple_m_euler_rotation);
        FieldFunctionTuple* field_function_tuple_m_is_serialization=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformOperator::set_m_is_serialization,
            &TypeFieldReflectionOparator::TypeTransformOperator::get_m_is_serialization,
            &TypeFieldReflectionOparator::TypeTransformOperator::getClassName,
            &TypeFieldReflectionOparator::TypeTransformOperator::getFieldName_m_is_serialization,
            &TypeFieldReflectionOparator::TypeTransformOperator::getFieldTypeName_m_is_serialization,
            &TypeFieldReflectionOparator::TypeTransformOperator::isArray_m_is_serialization);
        REGISTER_FIELD_TO_MAP("Transform", field_function_tuple_m_is_serialization);

        
        
        
        ClassFunctionTuple* class_function_tuple_Transform=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformOperator::getTransformBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeTransformOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeTransformOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("Transform", class_function_tuple_Transform);
    }
namespace TypeWrappersRegister{
    void Transform()
    {
        TypeWrapperRegister_Transform();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


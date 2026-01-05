#pragma once
#include "runtime/function/framework/component/transform/transform_component.h"

namespace VKernel{
    class TransformComponent;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeTransformComponentOperator{
    public:
        static const char* getClassName(){ return "TransformComponent";}
        static void* constructorWithJson(const Json& json_context){
            TransformComponent* ret_instance= new TransformComponent;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(TransformComponent*)instance);
        }
        // base class
        static int getTransformComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 1;
            out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
               out_list[i] = TypeMetaDef(VKernel::Component,static_cast<TransformComponent*>(instance));
            }
            return count;
        }
        // fields
        static const char* getFieldName_m_transform(){ return "m_transform";}
        static const char* getFieldTypeName_m_transform(){ return "Transform";}
        static void set_m_transform(void* instance, void* field_value){ static_cast<TransformComponent*>(instance)->m_transform = *static_cast<Transform*>(field_value);}
        static void* get_m_transform(void* instance){ return static_cast<void*>(&(static_cast<TransformComponent*>(instance)->m_transform));}
        static bool isArray_m_transform(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_TransformComponent(){
        FieldFunctionTuple* field_function_tuple_m_transform=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::set_m_transform,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::get_m_transform,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getFieldName_m_transform,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getFieldTypeName_m_transform,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::isArray_m_transform);
        REGISTER_FIELD_TO_MAP("TransformComponent", field_function_tuple_m_transform);

        
        
        
        ClassFunctionTuple* class_function_tuple_TransformComponent=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getTransformComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("TransformComponent", class_function_tuple_TransformComponent);
    }
namespace TypeWrappersRegister{
    void TransformComponent()
    {
        TypeWrapperRegister_TransformComponent();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


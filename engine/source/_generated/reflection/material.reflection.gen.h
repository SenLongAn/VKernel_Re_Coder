#pragma once
#include "runtime/resource/res_type/data/material.h"

namespace VKernel{
    class VKernel::MaterialRes;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeMaterialResOperator{
    public:
        static const char* getClassName(){ return "MaterialRes";}
        static void* constructorWithJson(const Json& json_context){
            VKernel::MaterialRes* ret_instance= new VKernel::MaterialRes;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(VKernel::MaterialRes*)instance);
        }
        // base class
        static int getMaterialResBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_base_colour_texture_file(){ return "m_base_colour_texture_file";}
        static const char* getFieldTypeName_m_base_colour_texture_file(){ return "std::string";}
        static void set_m_base_colour_texture_file(void* instance, void* field_value){ static_cast<VKernel::MaterialRes*>(instance)->m_base_colour_texture_file = *static_cast<std::string*>(field_value);}
        static void* get_m_base_colour_texture_file(void* instance){ return static_cast<void*>(&(static_cast<VKernel::MaterialRes*>(instance)->m_base_colour_texture_file));}
        static bool isArray_m_base_colour_texture_file(){ return false; }
        static const char* getFieldName_m_normal_texture_file(){ return "m_normal_texture_file";}
        static const char* getFieldTypeName_m_normal_texture_file(){ return "std::string";}
        static void set_m_normal_texture_file(void* instance, void* field_value){ static_cast<VKernel::MaterialRes*>(instance)->m_normal_texture_file = *static_cast<std::string*>(field_value);}
        static void* get_m_normal_texture_file(void* instance){ return static_cast<void*>(&(static_cast<VKernel::MaterialRes*>(instance)->m_normal_texture_file));}
        static bool isArray_m_normal_texture_file(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_MaterialRes(){
        FieldFunctionTuple* field_function_tuple_m_base_colour_texture_file=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMaterialResOperator::set_m_base_colour_texture_file,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::get_m_base_colour_texture_file,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::getFieldName_m_base_colour_texture_file,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::getFieldTypeName_m_base_colour_texture_file,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::isArray_m_base_colour_texture_file);
        REGISTER_FIELD_TO_MAP("MaterialRes", field_function_tuple_m_base_colour_texture_file);
        FieldFunctionTuple* field_function_tuple_m_normal_texture_file=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMaterialResOperator::set_m_normal_texture_file,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::get_m_normal_texture_file,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::getFieldName_m_normal_texture_file,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::getFieldTypeName_m_normal_texture_file,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::isArray_m_normal_texture_file);
        REGISTER_FIELD_TO_MAP("MaterialRes", field_function_tuple_m_normal_texture_file);

        
        
        
        ClassFunctionTuple* class_function_tuple_MaterialRes=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeMaterialResOperator::getMaterialResBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeMaterialResOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("MaterialRes", class_function_tuple_MaterialRes);
    }
namespace TypeWrappersRegister{
    void Material()
    {
        TypeWrapperRegister_MaterialRes();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


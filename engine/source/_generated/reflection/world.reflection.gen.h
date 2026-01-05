#pragma once
#include "runtime/resource/res_type/common/world.h"

namespace VKernel{
    class WorldRes;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeWorldResOperator{
    public:
        static const char* getClassName(){ return "WorldRes";}
        static void* constructorWithJson(const Json& json_context){
            WorldRes* ret_instance= new WorldRes;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(WorldRes*)instance);
        }
        // base class
        static int getWorldResBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_name(){ return "m_name";}
        static const char* getFieldTypeName_m_name(){ return "std::string";}
        static void set_m_name(void* instance, void* field_value){ static_cast<WorldRes*>(instance)->m_name = *static_cast<std::string*>(field_value);}
        static void* get_m_name(void* instance){ return static_cast<void*>(&(static_cast<WorldRes*>(instance)->m_name));}
        static bool isArray_m_name(){ return false; }
        static const char* getFieldName_m_level_urls(){ return "m_level_urls";}
        static const char* getFieldTypeName_m_level_urls(){ return "std::vector<std::string>";}
        static void set_m_level_urls(void* instance, void* field_value){ static_cast<WorldRes*>(instance)->m_level_urls = *static_cast<std::vector<std::string>*>(field_value);}
        static void* get_m_level_urls(void* instance){ return static_cast<void*>(&(static_cast<WorldRes*>(instance)->m_level_urls));}
        static bool isArray_m_level_urls(){ return true; }
        static const char* getFieldName_m_default_level_url(){ return "m_default_level_url";}
        static const char* getFieldTypeName_m_default_level_url(){ return "std::string";}
        static void set_m_default_level_url(void* instance, void* field_value){ static_cast<WorldRes*>(instance)->m_default_level_url = *static_cast<std::string*>(field_value);}
        static void* get_m_default_level_url(void* instance){ return static_cast<void*>(&(static_cast<WorldRes*>(instance)->m_default_level_url));}
        static bool isArray_m_default_level_url(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator
namespace ArrayReflectionOperator{
#ifndef ArraystdSSvectorLstdSSstringROperatorMACRO
#define ArraystdSSvectorLstdSSstringROperatorMACRO
    class ArraystdSSvectorLstdSSstringROperator{
        public:
            static const char* getArrayTypeName(){ return "std::vector<std::string>";}
            static const char* getElementTypeName(){ return "std::string";}
            static int getSize(void* instance){
                //todo: should check validation
                return static_cast<int>(static_cast<std::vector<std::string>*>(instance)->size());
            }
            static void* get(int index,void* instance){
                //todo: should check validation
                return static_cast<void*>(&((*static_cast<std::vector<std::string>*>(instance))[index]));
            }
            static void set(int index, void* instance, void* element_value){
                //todo: should check validation
                (*static_cast<std::vector<std::string>*>(instance))[index] = *static_cast<std::string*>(element_value);
            }
    };
#endif //ArraystdSSvectorLstdSSstringROperator
}//namespace ArrayReflectionOperator

    void TypeWrapperRegister_WorldRes(){
        FieldFunctionTuple* field_function_tuple_m_name=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldResOperator::set_m_name,
            &TypeFieldReflectionOparator::TypeWorldResOperator::get_m_name,
            &TypeFieldReflectionOparator::TypeWorldResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeWorldResOperator::getFieldName_m_name,
            &TypeFieldReflectionOparator::TypeWorldResOperator::getFieldTypeName_m_name,
            &TypeFieldReflectionOparator::TypeWorldResOperator::isArray_m_name);
        REGISTER_FIELD_TO_MAP("WorldRes", field_function_tuple_m_name);
        FieldFunctionTuple* field_function_tuple_m_level_urls=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldResOperator::set_m_level_urls,
            &TypeFieldReflectionOparator::TypeWorldResOperator::get_m_level_urls,
            &TypeFieldReflectionOparator::TypeWorldResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeWorldResOperator::getFieldName_m_level_urls,
            &TypeFieldReflectionOparator::TypeWorldResOperator::getFieldTypeName_m_level_urls,
            &TypeFieldReflectionOparator::TypeWorldResOperator::isArray_m_level_urls);
        REGISTER_FIELD_TO_MAP("WorldRes", field_function_tuple_m_level_urls);
        FieldFunctionTuple* field_function_tuple_m_default_level_url=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldResOperator::set_m_default_level_url,
            &TypeFieldReflectionOparator::TypeWorldResOperator::get_m_default_level_url,
            &TypeFieldReflectionOparator::TypeWorldResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeWorldResOperator::getFieldName_m_default_level_url,
            &TypeFieldReflectionOparator::TypeWorldResOperator::getFieldTypeName_m_default_level_url,
            &TypeFieldReflectionOparator::TypeWorldResOperator::isArray_m_default_level_url);
        REGISTER_FIELD_TO_MAP("WorldRes", field_function_tuple_m_default_level_url);

        
        
        ArrayFunctionTuple* array_tuple_stdSSvectorLstdSSstringR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<std::string>", array_tuple_stdSSvectorLstdSSstringR);
        ClassFunctionTuple* class_function_tuple_WorldRes=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldResOperator::getWorldResBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeWorldResOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeWorldResOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("WorldRes", class_function_tuple_WorldRes);
    }
namespace TypeWrappersRegister{
    void World()
    {
        TypeWrapperRegister_WorldRes();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


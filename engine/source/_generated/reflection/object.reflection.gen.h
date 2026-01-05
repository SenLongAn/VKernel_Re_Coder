#pragma once
#include "runtime/resource/res_type/common/object.h"

namespace VKernel{
    class ComponentDefinitionRes;
    class ObjectDefinitionRes;
    class ObjectInstanceRes;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeComponentDefinitionResOperator{
    public:
        static const char* getClassName(){ return "ComponentDefinitionRes";}
        static void* constructorWithJson(const Json& json_context){
            ComponentDefinitionRes* ret_instance= new ComponentDefinitionRes;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(ComponentDefinitionRes*)instance);
        }
        // base class
        static int getComponentDefinitionResBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_type_name(){ return "m_type_name";}
        static const char* getFieldTypeName_m_type_name(){ return "std::string";}
        static void set_m_type_name(void* instance, void* field_value){ static_cast<ComponentDefinitionRes*>(instance)->m_type_name = *static_cast<std::string*>(field_value);}
        static void* get_m_type_name(void* instance){ return static_cast<void*>(&(static_cast<ComponentDefinitionRes*>(instance)->m_type_name));}
        static bool isArray_m_type_name(){ return false; }
        static const char* getFieldName_m_component(){ return "m_component";}
        static const char* getFieldTypeName_m_component(){ return "std::string";}
        static void set_m_component(void* instance, void* field_value){ static_cast<ComponentDefinitionRes*>(instance)->m_component = *static_cast<std::string*>(field_value);}
        static void* get_m_component(void* instance){ return static_cast<void*>(&(static_cast<ComponentDefinitionRes*>(instance)->m_component));}
        static bool isArray_m_component(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_ComponentDefinitionRes(){
        FieldFunctionTuple* field_function_tuple_m_type_name=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::set_m_type_name,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::get_m_type_name,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::getFieldName_m_type_name,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::getFieldTypeName_m_type_name,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::isArray_m_type_name);
        REGISTER_FIELD_TO_MAP("ComponentDefinitionRes", field_function_tuple_m_type_name);
        FieldFunctionTuple* field_function_tuple_m_component=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::set_m_component,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::get_m_component,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::getFieldName_m_component,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::getFieldTypeName_m_component,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::isArray_m_component);
        REGISTER_FIELD_TO_MAP("ComponentDefinitionRes", field_function_tuple_m_component);

        
        
        
        ClassFunctionTuple* class_function_tuple_ComponentDefinitionRes=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::getComponentDefinitionResBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeComponentDefinitionResOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("ComponentDefinitionRes", class_function_tuple_ComponentDefinitionRes);
    }namespace TypeFieldReflectionOparator{
    class TypeObjectDefinitionResOperator{
    public:
        static const char* getClassName(){ return "ObjectDefinitionRes";}
        static void* constructorWithJson(const Json& json_context){
            ObjectDefinitionRes* ret_instance= new ObjectDefinitionRes;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(ObjectDefinitionRes*)instance);
        }
        // base class
        static int getObjectDefinitionResBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_components(){ return "m_components";}
        static const char* getFieldTypeName_m_components(){ return "std::vector<Reflection::ReflectionPtr<Component>>";}
        static void set_m_components(void* instance, void* field_value){ static_cast<ObjectDefinitionRes*>(instance)->m_components = *static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(field_value);}
        static void* get_m_components(void* instance){ return static_cast<void*>(&(static_cast<ObjectDefinitionRes*>(instance)->m_components));}
        static bool isArray_m_components(){ return true; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator
namespace ArrayReflectionOperator{
#ifndef ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperatorMACRO
#define ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperatorMACRO
    class ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator{
        public:
            static const char* getArrayTypeName(){ return "std::vector<Reflection::ReflectionPtr<Component>>";}
            static const char* getElementTypeName(){ return "Reflection::ReflectionPtr<Component>";}
            static int getSize(void* instance){
                //todo: should check validation
                return static_cast<int>(static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(instance)->size());
            }
            static void* get(int index,void* instance){
                //todo: should check validation
                return static_cast<void*>(&((*static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(instance))[index]));
            }
            static void set(int index, void* instance, void* element_value){
                //todo: should check validation
                (*static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(instance))[index] = *static_cast<Reflection::ReflectionPtr<Component>*>(element_value);
            }
    };
#endif //ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator
}//namespace ArrayReflectionOperator

    void TypeWrapperRegister_ObjectDefinitionRes(){
        FieldFunctionTuple* field_function_tuple_m_components=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeObjectDefinitionResOperator::set_m_components,
            &TypeFieldReflectionOparator::TypeObjectDefinitionResOperator::get_m_components,
            &TypeFieldReflectionOparator::TypeObjectDefinitionResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeObjectDefinitionResOperator::getFieldName_m_components,
            &TypeFieldReflectionOparator::TypeObjectDefinitionResOperator::getFieldTypeName_m_components,
            &TypeFieldReflectionOparator::TypeObjectDefinitionResOperator::isArray_m_components);
        REGISTER_FIELD_TO_MAP("ObjectDefinitionRes", field_function_tuple_m_components);

        
        
        ArrayFunctionTuple* array_tuple_stdSSvectorLReflectionSSReflectionPtrLComponentRR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<Reflection::ReflectionPtr<Component>>", array_tuple_stdSSvectorLReflectionSSReflectionPtrLComponentRR);
        ClassFunctionTuple* class_function_tuple_ObjectDefinitionRes=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeObjectDefinitionResOperator::getObjectDefinitionResBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeObjectDefinitionResOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeObjectDefinitionResOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("ObjectDefinitionRes", class_function_tuple_ObjectDefinitionRes);
    }namespace TypeFieldReflectionOparator{
    class TypeObjectInstanceResOperator{
    public:
        static const char* getClassName(){ return "ObjectInstanceRes";}
        static void* constructorWithJson(const Json& json_context){
            ObjectInstanceRes* ret_instance= new ObjectInstanceRes;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(ObjectInstanceRes*)instance);
        }
        // base class
        static int getObjectInstanceResBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_name(){ return "m_name";}
        static const char* getFieldTypeName_m_name(){ return "std::string";}
        static void set_m_name(void* instance, void* field_value){ static_cast<ObjectInstanceRes*>(instance)->m_name = *static_cast<std::string*>(field_value);}
        static void* get_m_name(void* instance){ return static_cast<void*>(&(static_cast<ObjectInstanceRes*>(instance)->m_name));}
        static bool isArray_m_name(){ return false; }
        static const char* getFieldName_m_definition(){ return "m_definition";}
        static const char* getFieldTypeName_m_definition(){ return "std::string";}
        static void set_m_definition(void* instance, void* field_value){ static_cast<ObjectInstanceRes*>(instance)->m_definition = *static_cast<std::string*>(field_value);}
        static void* get_m_definition(void* instance){ return static_cast<void*>(&(static_cast<ObjectInstanceRes*>(instance)->m_definition));}
        static bool isArray_m_definition(){ return false; }
        static const char* getFieldName_m_instanced_components(){ return "m_instanced_components";}
        static const char* getFieldTypeName_m_instanced_components(){ return "std::vector<Reflection::ReflectionPtr<Component>>";}
        static void set_m_instanced_components(void* instance, void* field_value){ static_cast<ObjectInstanceRes*>(instance)->m_instanced_components = *static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(field_value);}
        static void* get_m_instanced_components(void* instance){ return static_cast<void*>(&(static_cast<ObjectInstanceRes*>(instance)->m_instanced_components));}
        static bool isArray_m_instanced_components(){ return true; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator
namespace ArrayReflectionOperator{
#ifndef ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperatorMACRO
#define ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperatorMACRO
    class ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator{
        public:
            static const char* getArrayTypeName(){ return "std::vector<Reflection::ReflectionPtr<Component>>";}
            static const char* getElementTypeName(){ return "Reflection::ReflectionPtr<Component>";}
            static int getSize(void* instance){
                //todo: should check validation
                return static_cast<int>(static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(instance)->size());
            }
            static void* get(int index,void* instance){
                //todo: should check validation
                return static_cast<void*>(&((*static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(instance))[index]));
            }
            static void set(int index, void* instance, void* element_value){
                //todo: should check validation
                (*static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(instance))[index] = *static_cast<Reflection::ReflectionPtr<Component>*>(element_value);
            }
    };
#endif //ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator
}//namespace ArrayReflectionOperator

    void TypeWrapperRegister_ObjectInstanceRes(){
        FieldFunctionTuple* field_function_tuple_m_name=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::set_m_name,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::get_m_name,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getFieldName_m_name,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getFieldTypeName_m_name,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::isArray_m_name);
        REGISTER_FIELD_TO_MAP("ObjectInstanceRes", field_function_tuple_m_name);
        FieldFunctionTuple* field_function_tuple_m_definition=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::set_m_definition,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::get_m_definition,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getFieldName_m_definition,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getFieldTypeName_m_definition,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::isArray_m_definition);
        REGISTER_FIELD_TO_MAP("ObjectInstanceRes", field_function_tuple_m_definition);
        FieldFunctionTuple* field_function_tuple_m_instanced_components=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::set_m_instanced_components,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::get_m_instanced_components,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getFieldName_m_instanced_components,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getFieldTypeName_m_instanced_components,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::isArray_m_instanced_components);
        REGISTER_FIELD_TO_MAP("ObjectInstanceRes", field_function_tuple_m_instanced_components);

        
        
        ArrayFunctionTuple* array_tuple_stdSSvectorLReflectionSSReflectionPtrLComponentRR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<Reflection::ReflectionPtr<Component>>", array_tuple_stdSSvectorLReflectionSSReflectionPtrLComponentRR);
        ClassFunctionTuple* class_function_tuple_ObjectInstanceRes=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::getObjectInstanceResBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeObjectInstanceResOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("ObjectInstanceRes", class_function_tuple_ObjectInstanceRes);
    }
namespace TypeWrappersRegister{
    void Object()
    {
        TypeWrapperRegister_ComponentDefinitionRes();
    TypeWrapperRegister_ObjectDefinitionRes();
    TypeWrapperRegister_ObjectInstanceRes();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


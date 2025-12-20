#pragma once
#include "runtime/function/framework/component/component.h"

namespace VKernel{
    class Component;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeComponentOperator{
    public:
        static const char* getClassName(){ return "Component";}
        static void* constructorWithJson(const Json& json_context){
            Component* ret_instance= new Component;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(Component*)instance);
        }
        // base class
        static int getComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_Component(){
        

        
        
        
        ClassFunctionTuple* class_function_tuple_Component=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeComponentOperator::getComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("Component", class_function_tuple_Component);
    }
namespace TypeWrappersRegister{
    void Component()
    {
        TypeWrapperRegister_Component();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


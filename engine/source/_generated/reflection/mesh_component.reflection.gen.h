#pragma once
#include "runtime/function/framework/component/mesh/mesh_component.h"

namespace VKernel{
    class MeshComponent;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeMeshComponentOperator{
    public:
        static const char* getClassName(){ return "MeshComponent";}
        static void* constructorWithJson(const Json& json_context){
            MeshComponent* ret_instance= new MeshComponent;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(MeshComponent*)instance);
        }
        // base class
        static int getMeshComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 1;
            out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
               out_list[i] = TypeMetaDef(VKernel::Component,static_cast<MeshComponent*>(instance));
            }
            return count;
        }
        // fields
        static const char* getFieldName_m_mesh_res(){ return "m_mesh_res";}
        static const char* getFieldTypeName_m_mesh_res(){ return "MeshComponentRes";}
        static void set_m_mesh_res(void* instance, void* field_value){ static_cast<MeshComponent*>(instance)->m_mesh_res = *static_cast<MeshComponentRes*>(field_value);}
        static void* get_m_mesh_res(void* instance){ return static_cast<void*>(&(static_cast<MeshComponent*>(instance)->m_mesh_res));}
        static bool isArray_m_mesh_res(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_MeshComponent(){
        FieldFunctionTuple* field_function_tuple_m_mesh_res=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::set_m_mesh_res,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::get_m_mesh_res,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::getFieldName_m_mesh_res,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::getFieldTypeName_m_mesh_res,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::isArray_m_mesh_res);
        REGISTER_FIELD_TO_MAP("MeshComponent", field_function_tuple_m_mesh_res);

        
        
        
        ClassFunctionTuple* class_function_tuple_MeshComponent=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::getMeshComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("MeshComponent", class_function_tuple_MeshComponent);
    }
namespace TypeWrappersRegister{
    void MeshComponent()
    {
        TypeWrapperRegister_MeshComponent();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


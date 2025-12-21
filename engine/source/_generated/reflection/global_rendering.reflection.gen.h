#pragma once
#include "runtime/resource/res_type/global/global_rendering.h"

namespace VKernel{
    class GlobalRenderingRes;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeGlobalRenderingResOperator{
    public:
        static const char* getClassName(){ return "GlobalRenderingRes";}
        static void* constructorWithJson(const Json& json_context){
            GlobalRenderingRes* ret_instance= new GlobalRenderingRes;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(GlobalRenderingRes*)instance);
        }
        // base class
        static int getGlobalRenderingResBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_camera_config(){ return "m_camera_config";}
        static const char* getFieldTypeName_m_camera_config(){ return "CameraConfig";}
        static void set_m_camera_config(void* instance, void* field_value){ static_cast<GlobalRenderingRes*>(instance)->m_camera_config = *static_cast<CameraConfig*>(field_value);}
        static void* get_m_camera_config(void* instance){ return static_cast<void*>(&(static_cast<GlobalRenderingRes*>(instance)->m_camera_config));}
        static bool isArray_m_camera_config(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_GlobalRenderingRes(){
        FieldFunctionTuple* field_function_tuple_m_camera_config=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeGlobalRenderingResOperator::set_m_camera_config,
            &TypeFieldReflectionOparator::TypeGlobalRenderingResOperator::get_m_camera_config,
            &TypeFieldReflectionOparator::TypeGlobalRenderingResOperator::getClassName,
            &TypeFieldReflectionOparator::TypeGlobalRenderingResOperator::getFieldName_m_camera_config,
            &TypeFieldReflectionOparator::TypeGlobalRenderingResOperator::getFieldTypeName_m_camera_config,
            &TypeFieldReflectionOparator::TypeGlobalRenderingResOperator::isArray_m_camera_config);
        REGISTER_FIELD_TO_MAP("GlobalRenderingRes", field_function_tuple_m_camera_config);

        
        
        
        ClassFunctionTuple* class_function_tuple_GlobalRenderingRes=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeGlobalRenderingResOperator::getGlobalRenderingResBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeGlobalRenderingResOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeGlobalRenderingResOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("GlobalRenderingRes", class_function_tuple_GlobalRenderingRes);
    }
namespace TypeWrappersRegister{
    void GlobalRendering()
    {
        TypeWrapperRegister_GlobalRenderingRes();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


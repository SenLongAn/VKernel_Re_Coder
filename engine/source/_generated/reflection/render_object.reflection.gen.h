#pragma once
#include "runtime/function/render/render_object.h"

namespace VKernel{
    class VKernel::GameObjectMeshDesc;
    class VKernel::GameObjectMaterialDesc;
    class VKernel::GameObjectTransformDesc;
    class VKernel::GameObjectPartDesc;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeGameObjectMeshDescOperator{
    public:
        static const char* getClassName(){ return "GameObjectMeshDesc";}
        static void* constructorWithJson(const Json& json_context){
            VKernel::GameObjectMeshDesc* ret_instance= new VKernel::GameObjectMeshDesc;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(VKernel::GameObjectMeshDesc*)instance);
        }
        // base class
        static int getGameObjectMeshDescBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_mesh_file(){ return "m_mesh_file";}
        static const char* getFieldTypeName_m_mesh_file(){ return "std::string";}
        static void set_m_mesh_file(void* instance, void* field_value){ static_cast<VKernel::GameObjectMeshDesc*>(instance)->m_mesh_file = *static_cast<std::string*>(field_value);}
        static void* get_m_mesh_file(void* instance){ return static_cast<void*>(&(static_cast<VKernel::GameObjectMeshDesc*>(instance)->m_mesh_file));}
        static bool isArray_m_mesh_file(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_GameObjectMeshDesc(){
        FieldFunctionTuple* field_function_tuple_m_mesh_file=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectMeshDescOperator::set_m_mesh_file,
            &TypeFieldReflectionOparator::TypeGameObjectMeshDescOperator::get_m_mesh_file,
            &TypeFieldReflectionOparator::TypeGameObjectMeshDescOperator::getClassName,
            &TypeFieldReflectionOparator::TypeGameObjectMeshDescOperator::getFieldName_m_mesh_file,
            &TypeFieldReflectionOparator::TypeGameObjectMeshDescOperator::getFieldTypeName_m_mesh_file,
            &TypeFieldReflectionOparator::TypeGameObjectMeshDescOperator::isArray_m_mesh_file);
        REGISTER_FIELD_TO_MAP("GameObjectMeshDesc", field_function_tuple_m_mesh_file);

        
        
        
        ClassFunctionTuple* class_function_tuple_GameObjectMeshDesc=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectMeshDescOperator::getGameObjectMeshDescBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeGameObjectMeshDescOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeGameObjectMeshDescOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("GameObjectMeshDesc", class_function_tuple_GameObjectMeshDesc);
    }namespace TypeFieldReflectionOparator{
    class TypeGameObjectMaterialDescOperator{
    public:
        static const char* getClassName(){ return "GameObjectMaterialDesc";}
        static void* constructorWithJson(const Json& json_context){
            VKernel::GameObjectMaterialDesc* ret_instance= new VKernel::GameObjectMaterialDesc;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(VKernel::GameObjectMaterialDesc*)instance);
        }
        // base class
        static int getGameObjectMaterialDescBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_base_color_texture_file(){ return "m_base_color_texture_file";}
        static const char* getFieldTypeName_m_base_color_texture_file(){ return "std::string";}
        static void set_m_base_color_texture_file(void* instance, void* field_value){ static_cast<VKernel::GameObjectMaterialDesc*>(instance)->m_base_color_texture_file = *static_cast<std::string*>(field_value);}
        static void* get_m_base_color_texture_file(void* instance){ return static_cast<void*>(&(static_cast<VKernel::GameObjectMaterialDesc*>(instance)->m_base_color_texture_file));}
        static bool isArray_m_base_color_texture_file(){ return false; }
        static const char* getFieldName_m_normal_texture_file(){ return "m_normal_texture_file";}
        static const char* getFieldTypeName_m_normal_texture_file(){ return "std::string";}
        static void set_m_normal_texture_file(void* instance, void* field_value){ static_cast<VKernel::GameObjectMaterialDesc*>(instance)->m_normal_texture_file = *static_cast<std::string*>(field_value);}
        static void* get_m_normal_texture_file(void* instance){ return static_cast<void*>(&(static_cast<VKernel::GameObjectMaterialDesc*>(instance)->m_normal_texture_file));}
        static bool isArray_m_normal_texture_file(){ return false; }
        static const char* getFieldName_m_with_texture(){ return "m_with_texture";}
        static const char* getFieldTypeName_m_with_texture(){ return "bool";}
        static void set_m_with_texture(void* instance, void* field_value){ static_cast<VKernel::GameObjectMaterialDesc*>(instance)->m_with_texture = *static_cast<bool*>(field_value);}
        static void* get_m_with_texture(void* instance){ return static_cast<void*>(&(static_cast<VKernel::GameObjectMaterialDesc*>(instance)->m_with_texture));}
        static bool isArray_m_with_texture(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_GameObjectMaterialDesc(){
        FieldFunctionTuple* field_function_tuple_m_base_color_texture_file=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::set_m_base_color_texture_file,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::get_m_base_color_texture_file,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getClassName,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getFieldName_m_base_color_texture_file,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getFieldTypeName_m_base_color_texture_file,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::isArray_m_base_color_texture_file);
        REGISTER_FIELD_TO_MAP("GameObjectMaterialDesc", field_function_tuple_m_base_color_texture_file);
        FieldFunctionTuple* field_function_tuple_m_normal_texture_file=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::set_m_normal_texture_file,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::get_m_normal_texture_file,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getClassName,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getFieldName_m_normal_texture_file,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getFieldTypeName_m_normal_texture_file,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::isArray_m_normal_texture_file);
        REGISTER_FIELD_TO_MAP("GameObjectMaterialDesc", field_function_tuple_m_normal_texture_file);
        FieldFunctionTuple* field_function_tuple_m_with_texture=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::set_m_with_texture,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::get_m_with_texture,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getClassName,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getFieldName_m_with_texture,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getFieldTypeName_m_with_texture,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::isArray_m_with_texture);
        REGISTER_FIELD_TO_MAP("GameObjectMaterialDesc", field_function_tuple_m_with_texture);

        
        
        
        ClassFunctionTuple* class_function_tuple_GameObjectMaterialDesc=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::getGameObjectMaterialDescBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeGameObjectMaterialDescOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("GameObjectMaterialDesc", class_function_tuple_GameObjectMaterialDesc);
    }namespace TypeFieldReflectionOparator{
    class TypeGameObjectTransformDescOperator{
    public:
        static const char* getClassName(){ return "GameObjectTransformDesc";}
        static void* constructorWithJson(const Json& json_context){
            VKernel::GameObjectTransformDesc* ret_instance= new VKernel::GameObjectTransformDesc;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(VKernel::GameObjectTransformDesc*)instance);
        }
        // base class
        static int getGameObjectTransformDescBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_GameObjectTransformDesc(){
        

        
        
        
        ClassFunctionTuple* class_function_tuple_GameObjectTransformDesc=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectTransformDescOperator::getGameObjectTransformDescBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeGameObjectTransformDescOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeGameObjectTransformDescOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("GameObjectTransformDesc", class_function_tuple_GameObjectTransformDesc);
    }namespace TypeFieldReflectionOparator{
    class TypeGameObjectPartDescOperator{
    public:
        static const char* getClassName(){ return "GameObjectPartDesc";}
        static void* constructorWithJson(const Json& json_context){
            VKernel::GameObjectPartDesc* ret_instance= new VKernel::GameObjectPartDesc;
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*(VKernel::GameObjectPartDesc*)instance);
        }
        // base class
        static int getGameObjectPartDescBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_m_mesh_desc(){ return "m_mesh_desc";}
        static const char* getFieldTypeName_m_mesh_desc(){ return "GameObjectMeshDesc";}
        static void set_m_mesh_desc(void* instance, void* field_value){ static_cast<VKernel::GameObjectPartDesc*>(instance)->m_mesh_desc = *static_cast<GameObjectMeshDesc*>(field_value);}
        static void* get_m_mesh_desc(void* instance){ return static_cast<void*>(&(static_cast<VKernel::GameObjectPartDesc*>(instance)->m_mesh_desc));}
        static bool isArray_m_mesh_desc(){ return false; }
        static const char* getFieldName_m_material_desc(){ return "m_material_desc";}
        static const char* getFieldTypeName_m_material_desc(){ return "GameObjectMaterialDesc";}
        static void set_m_material_desc(void* instance, void* field_value){ static_cast<VKernel::GameObjectPartDesc*>(instance)->m_material_desc = *static_cast<GameObjectMaterialDesc*>(field_value);}
        static void* get_m_material_desc(void* instance){ return static_cast<void*>(&(static_cast<VKernel::GameObjectPartDesc*>(instance)->m_material_desc));}
        static bool isArray_m_material_desc(){ return false; }
        static const char* getFieldName_m_transform_desc(){ return "m_transform_desc";}
        static const char* getFieldTypeName_m_transform_desc(){ return "GameObjectTransformDesc";}
        static void set_m_transform_desc(void* instance, void* field_value){ static_cast<VKernel::GameObjectPartDesc*>(instance)->m_transform_desc = *static_cast<GameObjectTransformDesc*>(field_value);}
        static void* get_m_transform_desc(void* instance){ return static_cast<void*>(&(static_cast<VKernel::GameObjectPartDesc*>(instance)->m_transform_desc));}
        static bool isArray_m_transform_desc(){ return false; }

        // methods
        
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_GameObjectPartDesc(){
        FieldFunctionTuple* field_function_tuple_m_mesh_desc=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::set_m_mesh_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::get_m_mesh_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getClassName,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getFieldName_m_mesh_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getFieldTypeName_m_mesh_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::isArray_m_mesh_desc);
        REGISTER_FIELD_TO_MAP("GameObjectPartDesc", field_function_tuple_m_mesh_desc);
        FieldFunctionTuple* field_function_tuple_m_material_desc=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::set_m_material_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::get_m_material_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getClassName,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getFieldName_m_material_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getFieldTypeName_m_material_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::isArray_m_material_desc);
        REGISTER_FIELD_TO_MAP("GameObjectPartDesc", field_function_tuple_m_material_desc);
        FieldFunctionTuple* field_function_tuple_m_transform_desc=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::set_m_transform_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::get_m_transform_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getClassName,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getFieldName_m_transform_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getFieldTypeName_m_transform_desc,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::isArray_m_transform_desc);
        REGISTER_FIELD_TO_MAP("GameObjectPartDesc", field_function_tuple_m_transform_desc);

        
        
        
        ClassFunctionTuple* class_function_tuple_GameObjectPartDesc=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::getGameObjectPartDescBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeGameObjectPartDescOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("GameObjectPartDesc", class_function_tuple_GameObjectPartDesc);
    }
namespace TypeWrappersRegister{
    void RenderObject()
    {
        TypeWrapperRegister_GameObjectMeshDesc();
    TypeWrapperRegister_GameObjectMaterialDesc();
    TypeWrapperRegister_GameObjectTransformDesc();
    TypeWrapperRegister_GameObjectPartDesc();
    }
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace VKernel


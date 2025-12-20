#include "common/precompiled.h"

#include "generator/generator.h"
#include "language_types/class.h"

namespace Generator
{
    void GeneratorInterface::prepareStatus(std::string path)
    {
        if (!fs::exists(path)) ///< If the file does not exist
        {
            fs::create_directories(
                path); ///< Create the target folder: engine/source/_generated/reflection / serializer
        }
    }
    void GeneratorInterface::genClassRenderData(std::shared_ptr<Class> class_temp, Mustache::data& class_def)
    {
        // baise info
        class_def.set("class_name",
                      class_temp->getClassName()); ///< Get metadata information from the Class and write it to the data
        class_def.set("class_base_class_size", std::to_string(class_temp->m_base_classes.size()));
        class_def.set("class_need_register", true);

        // baise class
        if (class_temp->m_base_classes.size() > 0) ///< iterate
        {
            Mustache::data class_base_class_defines(Mustache::data::type::list);
            class_def.set("class_has_base", true);
            for (int index = 0; index < class_temp->m_base_classes.size(); ++index)
            {
                Mustache::data class_base_class_def;
                class_base_class_def.set("class_base_class_name", class_temp->m_base_classes[index]->name);
                class_base_class_def.set("class_base_class_index", std::to_string(index));
                class_base_class_defines.push_back(class_base_class_def);
            }
            class_def.set("class_base_class_defines", class_base_class_defines);
        }

        // field
        Mustache::data class_field_defines = Mustache::data::type::list;
        genClassFieldRenderData(class_temp, class_field_defines);
        class_def.set("class_field_defines", class_field_defines);

        // method
        Mustache::data class_method_defines = Mustache::data::type::list;
        genClassMethodRenderData(class_temp, class_method_defines);
        class_def.set("class_method_defines", class_method_defines);
    }
    void GeneratorInterface::genClassFieldRenderData(std::shared_ptr<Class> class_temp, Mustache::data& feild_defs)
    {
        static const std::string vector_prefix = "std::vector<";

        for (auto& field : class_temp->m_fields) ///< iterate
        {
            if (!field->shouldCompile())
                continue;
            Mustache::data filed_define;

            filed_define.set("class_field_name", field->m_name);
            filed_define.set("class_field_type", field->m_type);
            filed_define.set("class_field_display_name", field->m_display_name);
            bool is_vector = field->m_type.find(vector_prefix) == 0;
            filed_define.set("class_field_is_vector", is_vector);
            feild_defs.push_back(filed_define);
        }
    }

    void GeneratorInterface::genClassMethodRenderData(std::shared_ptr<Class> class_temp, Mustache::data& method_defs)
    {
        for (auto& method : class_temp->m_methods) ///< iterate
        {
            if (!method->shouldCompile())
                continue;
            Mustache::data method_define;

            method_define.set("class_method_name", method->m_name);
            method_defs.push_back(method_define);
        }
    }
} // namespace Generator

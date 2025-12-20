#include "common/precompiled.h"

#include "generator/reflection_generator.h"

#include "language_types/class.h"
#include "template_manager/template_manager.h"

#include <map>
#include <set>

namespace Generator
{
    ReflectionGenerator::ReflectionGenerator(std::string                             source_directory,
                                             std::function<std::string(std::string)> get_include_function) :
        GeneratorInterface(source_directory + "/_generated/reflection",
                           source_directory,
                           get_include_function) ///< Call the base class constructor, parameter0:
                                                 ///< engine/source/_generated/reflection, parameter1: Callback function
    {
        prepareStatus(m_out_path); ///< prepareStatus
    }
    void ReflectionGenerator::prepareStatus(std::string path)
    {
        GeneratorInterface::prepareStatus(path); // Create the target folder

        // Load template file
        TemplateManager::getInstance()->loadTemplates(m_root_path, "commonReflectionFile");
        TemplateManager::getInstance()->loadTemplates(m_root_path, "allReflectionFile");
        return;
    }

    std::string ReflectionGenerator::processFileName(std::string path)
    {
        auto relativeDir = fs::path(path).filename().replace_extension("reflection.gen.h").string();
        return m_out_path + "/" + relativeDir; ///< engine/source/_generated/reflection/xxxreflection.gen.h
    }

    int ReflectionGenerator::generate(std::string path, SchemaMoudle schema)
    {
        // ################################# Computational data ####################################
        // Traverse all Class in the current source file
        Mustache::data mustache_data;                             ///< Total Data
        Mustache::data class_defines(Mustache::data::type::list); ///< all class data

        static const std::string vector_prefix = "std::vector<";

        for (auto class_temp : schema.classes)
        {
            if (!class_temp->shouldCompile()) ///< Only process what's necessary to compile
                continue;

            // Obtain the metadata information of the current class and store it in class_def
            Mustache::data class_def; ///< current class data
            genClassRenderData(class_temp, class_def);

            // Handle vector type fields separately
            std::map<std::string, std::pair<std::string, std::string>>
                vector_map;                         ///< field type, <vectorLxxxR, Element type>
            for (auto field : class_temp->m_fields) ///< Traverse all fields
            {
                if (!field->shouldCompile()) ///< Only process what's necessary to compile
                    continue;

                bool is_array = field->m_type.find(vector_prefix) == 0; ///< Is it an array?
                if (is_array)
                {
                    // Calculate
                    std::string array_useful_name = field->m_type;

                    Utils::formatQualifiedName(array_useful_name);

                    std::string item_type = field->m_type;

                    item_type = Utils::getNameWithoutContainer(item_type);

                    vector_map[field->m_type] = std::make_pair(array_useful_name, item_type); ///< Add to vector_map
                }
            }
            Mustache::data vector_defines(Mustache::data::type::list);
            if (vector_map.size() > 0)
            {
                if (nullptr == class_def.get("vector_exist"))
                {
                    class_def.set("vector_exist", true); ///< Set this class to have vector members
                }
                for (auto vector_item : vector_map) ///< Traverse vector_map
                {
                    std::string array_useful_name = vector_item.second.first;  ///< vectorLxxxR
                    std::string item_type         = vector_item.second.second; ///< Element type

                    // set
                    Mustache::data vector_define;
                    vector_define.set("vector_useful_name", array_useful_name);
                    vector_define.set("vector_type_name", vector_item.first);
                    vector_define.set("vector_element_type_name", item_type);
                    vector_defines.push_back(vector_define);
                }
            }
            class_def.set("vector_defines", vector_defines);
            class_defines.push_back(class_def); ///< Add to class_defines
        }

        // set class defines
        mustache_data.set("class_defines", class_defines);

        // set headfile name:
        Mustache::data include_headfiles(Mustache::data::type::list);
        include_headfiles.push_back(
            Mustache::data("headfile_name",
                           Utils::makeRelativePath(m_root_path, path)
                               .string())); ///< Relative path from engine/source to the current source file
        mustache_data.set("include_headfiles", include_headfiles);

        std::string tmp = Utils::convertNameToUpperCamelCase(fs::path(path).stem().string(), "_");
        mustache_data.set("sourefile_name_upper_camel_case", tmp);

        // ################################# Render data to file ####################################
        std::string render_string = TemplateManager::getInstance()->renderByTemplate(
            "commonReflectionFile", mustache_data); ///< Data passed into template rendering

        std::string file_path = processFileName(path);
        Utils::saveFile(render_string, file_path); ///< Save the rendered content to the target file

        // Store data for use by all_files
        m_sourcefile_list.emplace_back(tmp); ///< The uppercase form of the source file name, with the '_' removed
        m_head_file_list.emplace_back(Utils::makeRelativePath(m_root_path, file_path)
                                          .string()); ///< Relative path from engine/source to the target file
        return 0;
    }
    void ReflectionGenerator::finish()
    {
        // ################################# Computational data ####################################
        Mustache::data mustache_data;
        Mustache::data include_headfiles = Mustache::data::type::list;
        Mustache::data sourefile_names   = Mustache::data::type::list;

        for (auto& head_file : m_head_file_list)
        {
            include_headfiles.push_back(Mustache::data("headfile_name", head_file));
        }
        for (auto& sourefile_name_upper_camel_case : m_sourcefile_list)
        {
            sourefile_names.push_back(
                Mustache::data("sourefile_name_upper_camel_case", sourefile_name_upper_camel_case));
        }
        mustache_data.set("include_headfiles", include_headfiles);
        mustache_data.set("sourefile_names", sourefile_names);

        // ################################# Render data to file ####################################
        std::string render_string =
            TemplateManager::getInstance()->renderByTemplate("allReflectionFile", mustache_data);
        Utils::saveFile(render_string, m_out_path + "/all_reflection.h");
    }

    ReflectionGenerator::~ReflectionGenerator() {}
} // namespace Generator
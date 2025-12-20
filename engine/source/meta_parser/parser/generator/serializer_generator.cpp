#include "generator/serializer_generator.h"
#include "common/precompiled.h"
#include "language_types/class.h"

namespace Generator
{
    SerializerGenerator::SerializerGenerator(std::string                             source_directory,
                                             std::function<std::string(std::string)> get_include_function) :
        GeneratorInterface(source_directory + "/_generated/serializer",
                           source_directory,
                           get_include_function) ///< Call the base class constructor, parameter0:
                                                 ///< engine/source/_generated/serializer, parameter1: Callback function
    {
        prepareStatus(m_out_path); ///< prepareStatus
    }

    void SerializerGenerator::prepareStatus(std::string path)
    {
        GeneratorInterface::prepareStatus(path); // Create the target folder

        // Load template file
        TemplateManager::getInstance()->loadTemplates(m_root_path, "allSerializer.h");
        TemplateManager::getInstance()->loadTemplates(m_root_path, "allSerializer.ipp");
        TemplateManager::getInstance()->loadTemplates(m_root_path, "commonSerializerGenFile");
        return;
    }

    std::string SerializerGenerator::processFileName(std::string path)
    {
        auto relativeDir = fs::path(path).filename().replace_extension("serializer.gen.h").string();
        return m_out_path + "/" + relativeDir; ///< engine/source/_generated/serializer/xxxserializer.gen.h
    }
    int SerializerGenerator::generate(std::string path, SchemaMoudle schema)
    {
        // ################################# Computational data ####################################
        std::string file_path = processFileName(path); ///< target file

        Mustache::data muatache_data;                             ///< Total Data
        Mustache::data class_defines(Mustache::data::type::list); ///< all class data

        // headfile name:
        Mustache::data include_headfiles(Mustache::data::type::list);
        include_headfiles.push_back(
            Mustache::data("headfile_name",
                           Utils::makeRelativePath(m_root_path, path)
                               .string())); ///< Relative path from engine/source to the current source file

        // Traverse all Class in the current source file
        for (auto class_temp : schema.classes)
        {
            if (!class_temp->shouldCompileFields()) ///< Only process what's necessary to compile
                continue;

            Mustache::data class_def;
            genClassRenderData(class_temp, class_def);

            // Traverse all base classes
            for (int index = 0; index < class_temp->m_base_classes.size(); ++index)
            {
                auto include_file = m_get_include_func(
                    class_temp->m_base_classes[index]->name); ///< Callback function, Pass in base class type, Return
                                                              ///< the source file path of the base class
                if (!include_file.empty())
                {
                    auto include_file_base = processFileName(include_file);
                    if (file_path != include_file_base)
                    {
                        // Relative path from engine/source to the basic source file
                        include_headfiles.push_back(Mustache::data(
                            "headfile_name", Utils::makeRelativePath(m_root_path, include_file_base).string()));
                    }
                }
            }

            // Traverse all fields
            for (auto field : class_temp->m_fields)
            {
                if (!field->shouldCompile()) ///< Only process what's necessary to compile
                    continue;

                if (field->m_type.find("std::vector") == 0) ///< Is it an array?
                {
                    auto include_file = m_get_include_func(field->m_name);
                    if (!include_file.empty())
                    {
                        auto include_file_base = processFileName(include_file);
                        if (file_path != include_file_base)
                        {
                            include_headfiles.push_back(Mustache::data(
                                "headfile_name", Utils::makeRelativePath(m_root_path, include_file_base).string()));
                        }
                    }
                }
            }
            // Add to class_defines
            class_defines.push_back(class_def);
            m_class_defines.push_back(class_def);
        }

        // set
        muatache_data.set("class_defines", class_defines);
        muatache_data.set("include_headfiles", include_headfiles);

        // ################################# Render data to file ####################################
        std::string render_string =
            TemplateManager::getInstance()->renderByTemplate("commonSerializerGenFile", muatache_data);
        Utils::saveFile(render_string, file_path);

        // Store data for use by all_files
        m_include_headfiles.push_back(
            Mustache::data("headfile_name", Utils::makeRelativePath(m_root_path, file_path).string()));
        return 0;
    }

    void SerializerGenerator::finish()
    {
        // ################################# Computational data ####################################
        Mustache::data mustache_data;
        mustache_data.set("class_defines", m_class_defines);
        mustache_data.set("include_headfiles", m_include_headfiles);

        // ################################# Render data to file ####################################
        std::string render_string = TemplateManager::getInstance()->renderByTemplate("allSerializer.h", mustache_data);
        Utils::saveFile(render_string, m_out_path + "/all_serializer.h");
        render_string = TemplateManager::getInstance()->renderByTemplate("allSerializer.ipp", mustache_data);
        Utils::saveFile(render_string, m_out_path + "/all_serializer.ipp");
    }

    SerializerGenerator::~SerializerGenerator() {}
} // namespace Generator

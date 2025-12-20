#pragma once
#include "generator/generator.h"

/**
 * Serializer Generator, Derived class
 */

namespace Generator
{
    class SerializerGenerator : public GeneratorInterface
    {
    public:
        // Construction
        SerializerGenerator() = delete;
        SerializerGenerator(std::string source_directory, std::function<std::string(std::string)> get_include_function);

        virtual int generate(std::string  path,
                             SchemaMoudle schema) override; ///< Generate files and store them in the _generated folder

        virtual void finish() override; ///< Generate files starting with 'all_'

        virtual ~SerializerGenerator() override; ///< Destruct

    protected:
        virtual void prepareStatus(std::string path) override; ///< Create the target folder and load the template file

        virtual std::string processFileName(std::string path) override; ///< Calculate target file path

    private:
        // Used for the generation of all_files
        Mustache::data m_class_defines {Mustache::data::type::list};
        Mustache::data m_include_headfiles {Mustache::data::type::list};
    };
} // namespace Generator

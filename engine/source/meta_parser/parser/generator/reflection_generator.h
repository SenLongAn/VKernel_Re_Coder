#pragma once
#include "generator/generator.h"

/**
 * Reflection Generator, Derived class
 */

namespace Generator
{
    class ReflectionGenerator : public GeneratorInterface
    {
    public:
        // Construction
        ReflectionGenerator() = delete;
        ReflectionGenerator(std::string source_directory, std::function<std::string(std::string)> get_include_function);

        virtual int  generate(std::string  path,
                              SchemaMoudle schema) override; ///< Generate files and store them in the _generated folder
        virtual void finish() override;                      ///< Generate files starting with 'all_'
        virtual ~ReflectionGenerator() override;             ///< Destruct

    protected:
        virtual void prepareStatus(std::string path) override; ///< Create the target folder and load the template file
        virtual std::string processFileName(std::string path) override; ///< Calculate target file path

    private:
        // Used for the generation of all_files
        std::vector<std::string> m_head_file_list; ///< Relative path from engine/source to the target file
        std::vector<std::string>
            m_sourcefile_list; ///< The uppercase form of the source file name, with the '_' removed
    };
} // namespace Generator

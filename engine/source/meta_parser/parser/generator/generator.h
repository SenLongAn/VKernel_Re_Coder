#pragma once
#include "common/schema_module.h"

#include <functional>
#include <string>

/**
 * Generator, create the final file, basic class
 */

namespace Generator
{
    class GeneratorInterface
    {
    public:
        // Construction
        GeneratorInterface(std::string                             out_path,
                           std::string                             root_path,
                           std::function<std::string(std::string)> get_include_func) :
            m_out_path(out_path), m_root_path(root_path), m_get_include_func(get_include_func) ///< Initialize member
        {}

        virtual int  generate(std::string  path,
                              SchemaMoudle schema) = 0; ///< Generate files and store them in the _generated folder
        virtual void finish() {};                       ///< Generate files starting with 'all_'

        virtual ~GeneratorInterface() {}; ///< Destruct

    protected:
        virtual void prepareStatus(std::string path); ///< Create the target folder and load the template file

        // Names to be replaced in the calculation template file
        virtual void genClassRenderData(std::shared_ptr<Class> class_temp, Mustache::data& class_def);
        virtual void genClassFieldRenderData(std::shared_ptr<Class> class_temp, Mustache::data& feild_defs);
        virtual void genClassMethodRenderData(std::shared_ptr<Class> class_temp, Mustache::data& method_defs);

        virtual std::string processFileName(std::string path) = 0; ///< Calculate target file path

        std::string m_out_path {"gen_src"}; ///< engine/source/_generated/reflection / serializer
        std::string m_root_path;            ///< engine/source/
        std::function<std::string(std::string)> m_get_include_func; ///< bind callback func: parser::getIncludeFile
    };
} // namespace Generator

#pragma once

#include "common/precompiled.h"

#include "common/namespace.h"
#include "common/schema_module.h"

#include "cursor/cursor.h"

#include "generator/generator.h"
#include "template_manager/template_manager.h"

/**
 * Parser
 * LLVM_Clang: Builds an AST (Abstract Syntax Tree) for the source in order to extract metadata information via cursors.
 * Mustache: Replaces the {{xxx}} content in template files based on the metadata info parsed by LLVM_Clang, thereby
 * generating new files.
 */

class Class;

class MetaParser
{
public:
    static void prepare(void); ///< Empty function body

    MetaParser(const std::string project_input_file,
               const std::string include_file_path,
               const std::string include_path,
               const std::string include_sys,
               const std::string module_name,
               bool              is_show_errors); ///< Construct, Create generator
    ~MetaParser(void);               ///< Destruct
    void finish(void);               ///< Generate files starting with 'all_'
    int  parse(void);                ///< Parse the contents of the source file into two unordered_maps
    void generateFiles(void);        ///< Generate files and store them in the _generated folder

private:
    // Initialized from main's argv parameter
    std::string m_project_input_file; // engine/bin/precompile.json, Generated while executing the precompile.cmake file
    std::string m_source_include_file_name;    // build/parser_header.h
    std::vector<std::string> m_work_paths;     // engine/source
    std::string              m_sys_include;    // *, Show indifference
    std::string              m_module_name;    // Vkernel
    bool                     m_is_show_errors; // 0. Indicates that errors will not be displayed

    CXIndex           m_index;
    CXTranslationUnit m_translation_unit;

    // The storage target for data parsed by the parser function
    std::unordered_map<std::string, std::string>
        m_type_table; // Type table: class type name, path of the source file where the class is located
    std::unordered_map<std::string, SchemaMoudle>
        m_schema_modules; // Source file path of the class, add Class（type
                          // info） to the classes of the SchemaModule object,Class is a vector because a file may have
                          // multiple class definitions.

    // Compilation command used by LLVM_Clang
    std::vector<const char*> arguments = {{"-x",
                                           "c++",                     // Specify language as C
                                           "-std=c++11",              // C 11 standard
                                           "-D__REFLECTION_PARSER__", // Custom macros (for reflection parsing)
                                           "-DNDEBUG",                // Disable debug assertions
                                           "-D__clang__",             // Define clang compiler macros
                                           "-w",                      // Disable all warnings
                                           "-MG", // Continue generating dependencies when headers are missing
                                           "-M",  // Generate dependencies
                                           "-ferror-limit=0",   // Do not limit the number of errors
                                           "-o clangLog.txt"}}; // Output file
    // Generator, used to create the final file
    std::vector<Generator::GeneratorInterface*> m_generators;

private:
    bool parseProject(void); // Generate paser_head.h
    void buildClassAST(const Cursor& cursor,
                       Namespace&    current_namespace); // Recursively traverse the AST tree to obtain metadata
                                                      // information and store it in the target map.
    std::string getIncludeFile(
        std::string name); // Callback function of the generator, Find the type name and return the source file path
};

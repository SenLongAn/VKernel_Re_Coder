#include "common/precompiled.h"

#include "language_types/class.h"

#include "generator/reflection_generator.h"
#include "generator/serializer_generator.h"

#include "parser.h"

// If it's a namespace, Add the current namespace to namespaces, recurse
#define RECURSE_NAMESPACES(kind, cursor, method, namespaces) \
    { \
        if (kind == CXCursor_Namespace) \
        { \
            auto display_name = cursor.getDisplayName(); \
            if (!display_name.empty()) \
            { \
                namespaces.emplace_back(display_name); \
                method(cursor, namespaces); \
                namespaces.pop_back(); \
            } \
        } \
    }

// If there are attributes of fields/methods, Get source file path
#define TRY_ADD_LANGUAGE_TYPE(handle, container) \
    { \
        if (handle->shouldCompile()) \
        { \
            auto file = handle->getSourceFile(); \
            m_schema_modules[file].container.emplace_back(handle); \
            m_type_table[handle->m_display_name] = file; \
        } \
    }

void MetaParser::prepare(void) {}

std::string MetaParser::getIncludeFile(std::string name)
{
    auto iter = m_type_table.find(name); ///<
    return iter == m_type_table.end() ? std::string() : iter->second;
}

MetaParser::MetaParser(const std::string project_input_file,
                       const std::string include_file_path,
                       const std::string include_path,
                       const std::string sys_include,
                       const std::string module_name,
                       bool              is_show_errors) :
    m_project_input_file(project_input_file), m_source_include_file_name(include_file_path), m_index(nullptr),
    m_translation_unit(nullptr), m_sys_include(sys_include), m_module_name(module_name),
    m_is_show_errors(is_show_errors) // Initialize members based on main's argv
{
    m_work_paths = Utils::split(include_path, ";");

    // Build Generator
    m_generators.emplace_back(new Generator::SerializerGenerator(
        m_work_paths[0], std::bind(&MetaParser::getIncludeFile, this, std::placeholders::_1))); // Serializer
    m_generators.emplace_back(new Generator::ReflectionGenerator(
        m_work_paths[0], std::bind(&MetaParser::getIncludeFile, this, std::placeholders::_1))); // Reflection
}

MetaParser::~MetaParser(void)
{
    for (auto item : m_generators)
    {
        delete item;
    }
    m_generators.clear();

    if (m_translation_unit)
        clang_disposeTranslationUnit(m_translation_unit);

    if (m_index)
        clang_disposeIndex(m_index);
}

void MetaParser::finish(void)
{
    for (auto generator_iter : m_generators)
    {
        generator_iter->finish();
    }
}

bool MetaParser::parseProject()
{
    bool result = true;
    std::cout << "Parsing project file: " << m_project_input_file << std::endl;

    // Open the precompile.json file in read-only mode
    std::fstream include_txt_file(m_project_input_file, std::ios::in);

    if (include_txt_file.fail())
    {
        std::cout << "Could not load file: " << m_project_input_file << std::endl;
        return false;
    }

    // Read content into a string
    std::stringstream buffer;
    buffer << include_txt_file.rdbuf();

    std::string context = buffer.str();

    auto inlcude_files = Utils::split(
        context, ";"); ///< Split the file content by ';' and store it in a std::vector<std::string> : source file paths
    std::fstream include_file;

    // Open the parser_header.h file for writing
    include_file.open(m_source_include_file_name, std::ios::out);
    if (!include_file.is_open())
    {
        std::cout << "Could not open the Source Include file: " << m_source_include_file_name << std::endl;
        return false;
    }

    std::cout << "Generating the Source Include file: " << m_source_include_file_name << std::endl;

    std::string output_filename = Utils::getFileName(m_source_include_file_name); ///< Get the filename: paser_header.h

    if (output_filename.empty())
    {
        output_filename = "META_INPUT_HEADER_H";
    }
    else
    {
        Utils::replace(output_filename, ".", "_");
        Utils::replace(output_filename, " ", "_");
        Utils::toUpper(output_filename);
    }

    // Iterate over each segmented path
    include_file << "#ifndef __" << output_filename << "__"
                 << std::endl; ///< Content inserted at the beginning of the document
    include_file << "#define __" << output_filename << "__" << std::endl;

    for (auto include_item : inlcude_files)
    {
        std::string temp_string(include_item);
        Utils::replace(temp_string, '\\', '/');
        include_file << "#include  \"" << temp_string << "\"" << std::endl;
    }

    include_file << "#endif" << std::endl; ///< Content inserted at the end of the file
    include_file.close();
    return result;
}

int MetaParser::parse(void)
{
    // parseProject: Generate paser_head.h
    bool parse_include_ = parseProject();
    if (!parse_include_)
    {
        std::cerr << "Parsing project file error! " << std::endl;
        return -1;
    }

    fs::path input_path(m_source_include_file_name); ///< Did parser_header.h parse successfully?
    if (!fs::exists(input_path))
    {
        std::cerr << input_path << " is not exist" << std::endl;
        return -2;
    }

    // Whether to display diagnostic information on the console
    std::cerr << "Parsing the whole project..." << std::endl;
    int is_show_errors = m_is_show_errors ? 1 : 0;
    m_index            = clang_createIndex(true, is_show_errors); ///< is_show_errors

    // Compilation command used by LLVM_Clang
    std::string pre_include = "-I";
    std::string sys_include_temp;
    if (!(m_sys_include == "*")) ///< The Windows system will not execute this.
    {
        sys_include_temp = pre_include + m_sys_include;
        arguments.emplace_back(sys_include_temp.c_str());
    }

    auto paths = m_work_paths;
    for (int index = 0; index < paths.size(); ++index)
    {
        paths[index] = pre_include + paths[index];

        arguments.emplace_back(paths[index].c_str()); ///< -I: Header file search path: engine/source
    }

    // Build an AST tree using LLVM_Clang
    m_translation_unit = clang_createTranslationUnitFromSourceFile(
        m_index, m_source_include_file_name.c_str(), static_cast<int>(arguments.size()), arguments.data(), 0, nullptr);
    auto cursor = clang_getTranslationUnitCursor(m_translation_unit); ///< Get root node pointer

    // Recursively traverse the AST tree to obtain metadata, information and store it in the target map.
    Namespace temp_namespace; ///< Used to record the namespace where the current meta resides. A vector is used because
                              ///< namespaces can be nested.

    buildClassAST(cursor,
                  temp_namespace); ///< Recursively traverse the AST from the root node, get metadata information

    temp_namespace.clear();

    return 0;
}

void MetaParser::generateFiles(void)
{
    std::cerr << "Start generate runtime schemas(" << m_schema_modules.size() << ")..." << std::endl;
    for (auto& schema : m_schema_modules) ///< Traverse all metadata according to the files
    {
        for (auto& generator_iter : m_generators) ///< Iterate over two generators
        {
            generator_iter->generate(schema.first, schema.second); ///< Generate File
        }
    }

    finish();
}

void MetaParser::buildClassAST(const Cursor& cursor, Namespace& current_namespace)
{
    for (auto& child : cursor.getChildren()) ///< Traverse all direct nodes
    {
        auto kind = child.getKind();

        // actual definition and a class or struct
        if (child.isDefinition() &&
            (kind == CXCursor_ClassDecl ||
             kind == CXCursor_StructDecl)) ///< If the node the cursor points to is a definition rather than a
                                           ///< declaration, and it is a class/struct definition
        {
            auto class_ptr = std::make_shared<Class>(child, current_namespace); ///< Create Class

            TRY_ADD_LANGUAGE_TYPE(class_ptr, classes); ///< Data added to target
        }
        else ///< If it's a namespace, then recurse
        {
            RECURSE_NAMESPACES(kind, child, buildClassAST, current_namespace);
        }
    }
}

#pragma once

#include "common/namespace.h"
#include "cursor/cursor.h"

/**
 * util funcs
 */

namespace Utils
{
    // Convert Clang's CXString to std::string
    void toString(const CXString& str, std::string& output);

    // Get the raw text of a type (qualified name)
    std::string getQualifiedName(const CursorType& type);

    // Combine name and namespace into a qualified string
    std::string getQualifiedName(const std::string& display_name, const Namespace& current_namespace);

    // Combine cursor's identifier with namespace
    std::string getQualifiedName(const Cursor& cursor, const Namespace& current_namespace);

    // Format qualified name by replacing special characters for code generation
    std::string formatQualifiedName(std::string& source_string);

    // Calculate relative path from 'from' to 'to' and return it
    fs::path makeRelativePath(const fs::path& from, const fs::path& to);

    // Output error message and exit program
    void fatalError(const std::string& error);

    // Compare if two iterator ranges are equal
    template<typename A, typename B>
    bool rangeEqual(A startA, A endA, B startB, B endB);

    // Split string into substrings array by specified delimiter
    std::vector<std::string> split(std::string input, std::string pat);

    // Extract filename from file path
    std::string getFileName(std::string path);

    // Remove "m_" prefix from member variable name
    std::string getNameWithoutFirstM(std::string& name);

    // Get type name without namespace part
    std::string getTypeNameWithoutNamespace(const CursorType& type);

    // Extract inner type from container type name (e.g., extract "int" from "vector<int>")
    std::string getNameWithoutContainer(std::string name);

    // Remove quotation marks from both ends of a string
    std::string getStringWithoutQuot(std::string input);

    // Replace substring in source string with new string
    std::string replace(std::string& source_string, std::string sub_string, const std::string new_string);

    // Replace specified character with new character in string
    std::string replace(std::string& source_string, char taget_char, const char new_char);

    // Convert string to uppercase
    std::string toUpper(std::string& source_string);

    // Join string array into one string with specified separator
    std::string join(std::vector<std::string> context_list, std::string separator);

    // Remove specified characters from both beginning and end of string
    std::string trim(std::string& source_string, const std::string trim_chars);

    // Read file content and return as string
    std::string loadFile(std::string path);

    // Save string content to file
    void saveFile(const std::string& outpu_string, const std::string& output_file);

    // Replace all matching substrings in string
    void replaceAll(std::string& resource_str, std::string sub_str, std::string new_str);

    // Format path: remove ".." and "." segments
    unsigned long formatPathString(const std::string& path_string, std::string& out_string);

    // Convert string to UpperCamelCase naming convention (e.g., "my_name" -> "MyName")
    std::string convertNameToUpperCamelCase(const std::string& name, std::string pat);
} // namespace Utils

#include "meta_utils.hpp"

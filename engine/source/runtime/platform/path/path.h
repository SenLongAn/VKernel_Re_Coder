#pragma once

#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

/**
 * path
 */
namespace VKernel
{
    class Path
    {
    public:
        static const std::filesystem::path
        getRelativePath(const std::filesystem::path& directory,
                        const std::filesystem::path& file_path); ///< get Relative Path

        static const std::vector<std::string>
        getPathSegments(const std::filesystem::path& file_path); ///< Split file_path into segments

        static const std::tuple<std::string, std::string, std::string>
        getFileExtensions(const std::filesystem::path& file_path); ///< Get multi-level extensions

        static const std::string getFilePureName(const std::string); ///< Get the filename without the extension
    };
} // namespace VKernel
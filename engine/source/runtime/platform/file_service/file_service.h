#pragma once

#include <filesystem>
#include <vector>

/**
 * File
 */

namespace VKernel
{
    class FileSystem
    {
    public:
        std::vector<std::filesystem::path>
        getFiles(const std::filesystem::path& directory); ///< Get all subdirectories of a directory
    };
} // namespace VKernel
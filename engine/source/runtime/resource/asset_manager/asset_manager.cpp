#include "runtime/resource/asset_manager/asset_manager.h"

#include "runtime/resource/config_manager/config_manager.h"

#include "runtime/function/global/global_context.h"

#include <filesystem>
#include <iostream>
namespace VKernel
{
    std::filesystem::path AssetManager::getFullPath(const std::string& relative_path) const
    {
        return std::filesystem::absolute(
            g_runtime_global_context.m_config_manager->getRootFolder() /
            relative_path); ///< bin/asset/...， If the right side of / is an absolute path (starting with a drive
                            ///< letter), ignore the left side of / and return the right side directly.
    }
} // namespace VKernel
#pragma once

#include <filesystem>

namespace VKernel
{
    struct EngineInitParams;

    class ConfigManager
    {
    public:
        void initialize(const std::filesystem::path& config_file_path); ///< init

        // get path
        const std::filesystem::path& getRootFolder() const;
        const std::filesystem::path& getAssetFolder() const;
        const std::string&           getGlobalRenderingResUrl() const;

    private:
        // path
        std::filesystem::path m_root_folder;
        std::filesystem::path m_asset_folder;
        std::string           m_global_rendering_res_url;
    };
} // namespace VKernel

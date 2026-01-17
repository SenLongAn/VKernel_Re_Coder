#pragma once

#include "runtime/core/meta/serializer/serializer.h"

#include "runtime/core/base/macro.h"

#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>

#include "_generated/serializer/all_serializer.h" ///< Specialized version

/**
 * runtime serialization and deserialization interface
 */

namespace VKernel
{
    class AssetManager
    {
    public:
        // Deserialization
        template<typename AssetType>
        bool loadAsset(const std::string& asset_url, AssetType& out_asset) const
        {
            // Open the asset_url file and read its contents into a string
            std::filesystem::path asset_path = getFullPath(asset_url);
            std::ifstream         asset_json_file(asset_path);
            if (!asset_json_file)
            {
                LOG_ERROR("open file: failed!");
                return false;
            }

            std::stringstream buffer;
            buffer << asset_json_file.rdbuf();
            std::string asset_json_text(buffer.str());

            // Call the specialized version of the deserialization function to reconstruct the object out_asset
            std::string error;
            auto&&      asset_json = Json::parse(asset_json_text, error);
            if (!error.empty())
            {
                LOG_ERROR("parse json file failed!");
                return false;
            }

            Serializer::read(asset_json, out_asset);
            return true;
        }

        // Serialization
        template<typename AssetType>
        bool saveAsset(const AssetType& out_asset, const std::string& asset_url) const
        {
            // Open the output file asset_url
            std::ofstream asset_json_file(getFullPath(asset_url));
            if (!asset_json_file)
            {
                LOG_ERROR("open file failed!");
                return false;
            }

            // Call the specialized version of the serialization function to write JSON data
            auto&&        asset_json      = Serializer::write(out_asset);
            std::string&& asset_json_text = asset_json.dump();

            asset_json_file << asset_json_text;
            asset_json_file.flush();

            return true;
        }

        // Takes a relative path and returns the absolute path
        std::filesystem::path getFullPath(const std::string& relative_path) const;
    };
} // namespace VKernel

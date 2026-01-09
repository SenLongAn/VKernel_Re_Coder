#include "editor/include/editor_file_service.h"

#include "runtime/platform/file_service/file_service.h"
#include "runtime/platform/path/path.h"

#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/config_manager/config_manager.h"

#include "runtime/function/global/global_context.h"

namespace ReCoder
{
    void EditorFileService::buildEngineFileTree()
    {
        // Find all subdirectories of the asset and store them in a vector
        std::string asset_folder =
            VKernel::g_runtime_global_context.m_config_manager->getAssetFolder().generic_string();
        const std::vector<std::filesystem::path> file_paths =
            VKernel::g_runtime_global_context.m_file_system->getFiles(asset_folder);
        std::vector<std::vector<std::string>>
            all_file_segments; ///< An internal vector stores the segment divisions of each path
        for (const auto& path : file_paths)
        {
            const std::filesystem::path& relative_path = VKernel::Path::getRelativePath(asset_folder, path);
            all_file_segments.emplace_back(VKernel::Path::getPathSegments(relative_path));
        }

        std::vector<std::shared_ptr<EditorFileNode>> node_array; ///< Storage of each segment node of the path
        m_file_node_array.clear();

        // root
        auto root_node = std::make_shared<EditorFileNode>();
        *root_node     = m_root_node;
        m_file_node_array.push_back(root_node);

        // Iterate through all subdirectories
        int all_file_segments_count = all_file_segments.size();
        for (int file_index = 0; file_index < all_file_segments_count; file_index++) ///< Traverse each path
        {
            int depth = 0; ///< The depth of each segment in the subdirectory is 1
            node_array.clear();
            node_array.push_back(root_node);

            int file_segment_count = all_file_segments[file_index].size();
            for (int file_segment_index = 0; file_segment_index < file_segment_count;
                 file_segment_index++) ///< Traverse each segment
            {
                auto file_node = std::make_shared<EditorFileNode>(); ///< create new node

                // set
                file_node->m_file_name = all_file_segments[file_index][file_segment_index]; ///< name
                if (depth < file_segment_count - 1)
                {
                    file_node->m_file_type = "Folder"; ///< type
                }
                else
                {
                    const auto& extensions = VKernel::Path::getFileExtensions(file_paths[file_index]);
                    file_node->m_file_type = std::get<0>(extensions);
                    if (file_node->m_file_type.size() == 0)
                        continue;

                    if (file_node->m_file_type.compare(".json") == 0)
                    {
                        file_node->m_file_type = std::get<1>(extensions); ///< type
                        if (file_node->m_file_type.compare(".component") == 0)
                        {
                            file_node->m_file_type = std::get<2>(extensions) + std::get<1>(extensions); ///< type
                        }
                    }
                    file_node->m_file_type = file_node->m_file_type.substr(1);        ///< type
                    file_node->m_file_path = file_paths[file_index].generic_string(); ///< path
                }
                file_node->m_node_depth = depth; ///< depth
                node_array.push_back(file_node);

                // push
                bool node_exists = checkFileArray(file_node.get()); ///< Check if it has been added
                if (node_exists == false)
                {
                    m_file_node_array.push_back(file_node); ///< push arr
                }
                EditorFileNode* parent_node_ptr = getParentNodePtr(node_array[depth].get());
                if (parent_node_ptr != nullptr && node_exists == false)
                {
                    parent_node_ptr->m_child_nodes.push_back(file_node); ///< Set the child node of the parent node
                }

                depth++;
            }
        }
    }

    bool EditorFileService::checkFileArray(EditorFileNode* file_node)
    {
        int editor_node_count = m_file_node_array.size();
        for (int file_node_index = 0; file_node_index < editor_node_count;
             file_node_index++) ///< Traverse m_file_node_array
        {
            if (m_file_node_array[file_node_index]->m_file_name == file_node->m_file_name &&
                m_file_node_array[file_node_index]->m_node_depth ==
                    file_node->m_node_depth) ///< Both the name and depth are the same, there can only be one unique
                                             ///< name at the same depth
            {
                return true;
            }
        }
        return false;
    }

    EditorFileNode* EditorFileService::getParentNodePtr(EditorFileNode* file_node)
    {
        int editor_node_count = m_file_node_array.size();
        for (int file_node_index = 0; file_node_index < editor_node_count;
             file_node_index++) ///< Traverse m_file_node_array
        {
            if (m_file_node_array[file_node_index]->m_file_name == file_node->m_file_name &&
                m_file_node_array[file_node_index]->m_node_depth ==
                    file_node->m_node_depth) ///< Both the name and depth are the same,
            {
                return m_file_node_array[file_node_index].get();
            }
        }
        return nullptr;
    }
} // namespace ReCoder

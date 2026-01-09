#pragma once

#include <memory>
#include <string>
#include <vector>

/**
 * File Tree Builder
 */
namespace ReCoder
{
    class EditorFileNode;
    using EditorFileNodeArray = std::vector<std::shared_ptr<EditorFileNode>>; ///< child nodes

    struct EditorFileNode ///< node
    {
        std::string         m_file_name;   ///< file name
        std::string         m_file_type;   ///< file type
        std::string         m_file_path;   ///< file path
        int                 m_node_depth;  ///< node depth
        EditorFileNodeArray m_child_nodes; ///< child nodes

        // Construct
        EditorFileNode() = default;
        EditorFileNode(const std::string& name, const std::string& type, const std::string& path, int depth) :
            m_file_name(name), m_file_type(type), m_file_path(path), m_node_depth(depth)
        {}
    };

    class EditorFileService ///< File Tree Builder
    {
        EditorFileNodeArray m_file_node_array; ///< All nodes are stored here

        EditorFileNode m_root_node {"asset", "Folder", "asset", -1}; ///< root

    private:
        EditorFileNode* getParentNodePtr(EditorFileNode* file_node); ///< get parent node
        bool            checkFileArray(EditorFileNode* file_node);   ///< Whether in m_file_node_array

    public:
        EditorFileNode* getEditorRootNode()
        {
            return m_file_node_array.empty() ? nullptr : m_file_node_array[0].get();
        } ///< get root node

        void buildEngineFileTree(); ///< Build File Tree
    };
} // namespace ReCoder

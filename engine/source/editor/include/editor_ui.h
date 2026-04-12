#pragma once

#include "runtime/core/math/vector2.h"
#include "runtime/function/ui/window_ui.h"

#include "editor/include/editor_file_service.h"

/**
 * Editor UI Settings
 */

namespace VKernel
{
    class WindowUI;
}

namespace ReCoder
{
    class EditorUI : public VKernel::WindowUI
    {
        WINDOWUI_CLASS(EditorUI);

    public:
        EditorUI(bool isGameMode);

        virtual void initialize(VKernel::WindowUIInitInfo init_info) override final; ///< init
        virtual void preUpdate() override final;                                     ///< pre render
        virtual void preRender() override final;                                     ///< render ui

        void
        drawAxisToggleButton(const char *string_id, bool check_state, int axis_mode); ///< render button and gizmo axis

        void buildEditorFileAssetsUITree(EditorFileNode *node); ///< build ui tree

    private:
        bool m_on_game_window = false;

        EditorFileService m_editor_file_service;

        std::unordered_map<std::string, unsigned int> m_new_object_index_map; ///< file name, count
        std::unordered_map<std::string, std::function<void(std::string, void *)>>
            m_editor_ui_creator; ///< function name, function

    private:
        // show editor window
        void showEditorUI();

        void showEditorMenu();
        void showEditorWorldObjectsWindow();
        void showEditorFileContentWindow();
        void showEditorGameWindow();
        void showEditorDetailWindow();

        // UI color
        void setUIColorStyle();

        // click file content leaf node
        void onFileContentItemClicked(EditorFileNode *node);

        // get
        std::string getLeafUINodeParentLabel();

        // detail node
        void createClassUI(VKernel::Reflection::ReflectionInstance &instance);
        void createLeafNodeUI(VKernel::Reflection::ReflectionInstance &instance);
    };
} // namespace ReCoder
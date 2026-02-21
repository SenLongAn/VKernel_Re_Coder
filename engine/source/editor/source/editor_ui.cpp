#include "editor/include/editor_ui.h"

#include "editor/include/editor_global_context.h"

#include "editor/include/editor_input_manager.h"
#include "editor/include/editor_scene_manager.h"
#include "runtime/engine.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/render/render_system.h"
#include "runtime/platform/path/path.h"
#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/config_manager/config_manager.h"

#include "runtime/function/render/window_system.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <array>
#include <filesystem>

namespace ReCoder
{
    WINDOWUI_REGISTER(EditorUI, false);

    std::vector<std::pair<std::string, bool>> g_editor_node_state_array; ///< treeNodeName, Is it expanded
    int                                       g_node_depth = -1;         ///< TreeNode Depth

    void DrawVecControl(const std::string& label,
                        VKernel::Vector3&  values,
                        float              resetValue  = 0.0f,
                        float              columnWidth = 100.0f);

    EditorUI::EditorUI(bool isGameMode)
    {
        m_only_game_mode = isGameMode;

        // TreeNodePush
        const auto& asset_folder            = VKernel::g_runtime_global_context.m_config_manager->getAssetFolder();
        m_editor_ui_creator["TreeNodePush"] = [this](const std::string& name, void* value_ptr) -> void { ///< nodeName
            static ImGuiTableFlags flags      = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings;
            bool                   node_state = false;
            g_node_depth++;
            if (g_node_depth > 0) ///< If it's not the root node
            {
                if (g_editor_node_state_array[g_node_depth - 1].second) ///< Parent node expanded
                {
                    node_state = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth); ///< Create Node
                }
                else
                {
                    g_editor_node_state_array.emplace_back(std::pair(name.c_str(), node_state));
                    return;
                }
            }
            else ///< If it is a root node
            {
                node_state = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
            }
            g_editor_node_state_array.emplace_back(std::pair(name.c_str(), node_state));
        };

        // TreeNodePop
        m_editor_ui_creator["TreeNodePop"] = [this](const std::string& name, void* value_ptr) -> void {
            if (g_editor_node_state_array[g_node_depth].second) ///< node expanded
            {
                ImGui::TreePop(); ///< pop
            }
            g_editor_node_state_array.pop_back(); ///< pop
            g_node_depth--;
        };

        // Transform
        m_editor_ui_creator["Transform"] = [this](const std::string& name,
                                                  void* value_ptr) -> void { ///< feild name, field instance
            if (g_editor_node_state_array[g_node_depth].second)
            {
                VKernel::Transform* trans_ptr = static_cast<VKernel::Transform*>(value_ptr);

                // rotation : Quaternion -> vec3
                VKernel::Vector3 degrees_val;
                degrees_val.x = trans_ptr->m_rotation.getPitch(false).valueDegrees();
                degrees_val.y = trans_ptr->m_rotation.getRoll(false).valueDegrees();
                degrees_val.z = trans_ptr->m_rotation.getYaw(false).valueDegrees();

                // Draw Control
                DrawVecControl("Position", trans_ptr->m_position);
                DrawVecControl("Rotation", degrees_val);
                DrawVecControl("Scale", trans_ptr->m_scale);

                // rotation : new vec3 -> Quaternion
                trans_ptr->m_rotation.w = VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.x / 2)) *
                                              VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.y / 2)) *
                                              VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.z / 2)) +
                                          VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.x / 2)) *
                                              VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.y / 2)) *
                                              VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.z / 2));
                trans_ptr->m_rotation.x = VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.x / 2)) *
                                              VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.y / 2)) *
                                              VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.z / 2)) -
                                          VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.x / 2)) *
                                              VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.y / 2)) *
                                              VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.z / 2));
                trans_ptr->m_rotation.y = VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.x / 2)) *
                                              VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.y / 2)) *
                                              VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.z / 2)) +
                                          VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.x / 2)) *
                                              VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.y / 2)) *
                                              VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.z / 2));
                trans_ptr->m_rotation.z = VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.x / 2)) *
                                              VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.y / 2)) *
                                              VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.z / 2)) -
                                          VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.x / 2)) *
                                              VKernel::Math::sin(VKernel::Math::degreesToRadians(degrees_val.y / 2)) *
                                              VKernel::Math::cos(VKernel::Math::degreesToRadians(degrees_val.z / 2));
                trans_ptr->m_rotation.normalise();

                // update gizmo axis
                g_editor_global_context.m_scene_manager->drawSelectedEntityAxis();
                g_editor_global_context.m_scene_manager->setSelectedObjectMatrix(trans_ptr->getMatrix());
            }
        };

        // bool
        m_editor_ui_creator["bool"] = [this](const std::string& name, void* value_ptr) -> void { ///< feild name, field
            if (g_node_depth == -1) ///< If it is a tree structure
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str()); ///< field name
                ImGui::SameLine();
                ImGui::Checkbox(label.c_str(), static_cast<bool*>(value_ptr)); ///< Checkbox
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name; ///< parent name + current name
                    ImGui::Text("%s", name.c_str());
                    ImGui::Checkbox(full_label.c_str(), static_cast<bool*>(value_ptr));
                }
            }
        };

        // int
        m_editor_ui_creator["int"] = [this](const std::string& name, void* value_ptr) -> void {
            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::InputInt(label.c_str(), static_cast<int*>(value_ptr)); ///< InputInt
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    ImGui::InputInt(full_label.c_str(), static_cast<int*>(value_ptr));
                }
            }
        };

        // float
        m_editor_ui_creator["float"] = [this](const std::string& name, void* value_ptr) -> void {
            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::InputFloat(label.c_str(), static_cast<float*>(value_ptr)); ///< InputFloat
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    ImGui::InputFloat(full_label.c_str(), static_cast<float*>(value_ptr));
                }
            }
        };

        // Vector3
        m_editor_ui_creator["Vector3"] = [this](const std::string& name, void* value_ptr) -> void {
            VKernel::Vector3* vec_ptr = static_cast<VKernel::Vector3*>(value_ptr);

            // vec3 -> float [3]
            float val[3] = {vec_ptr->x, vec_ptr->y, vec_ptr->z};

            //
            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::DragFloat3(label.c_str(), val); ///< DragFloat3
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    ImGui::DragFloat3(full_label.c_str(), val);
                }
            }

            // float [3] -> vec3
            vec_ptr->x = val[0];
            vec_ptr->y = val[1];
            vec_ptr->z = val[2];
        };

        // Quaternion
        m_editor_ui_creator["Quaternion"] = [this](const std::string& name, void* value_ptr) -> void {
            VKernel::Quaternion* qua_ptr = static_cast<VKernel::Quaternion*>(value_ptr);

            // vec4 -> float [4]
            float val[4] = {qua_ptr->x, qua_ptr->y, qua_ptr->z, qua_ptr->w};

            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::DragFloat4(label.c_str(), val);
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    ImGui::DragFloat4(full_label.c_str(), val);
                }
            }

            // float [4] -> vec4
            qua_ptr->x = val[0];
            qua_ptr->y = val[1];
            qua_ptr->z = val[2];
            qua_ptr->w = val[3];
        };

        // string
        m_editor_ui_creator["std::string"] = [this, &asset_folder](const std::string& name, void* value_ptr) -> void {
            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::Text("%s", (*static_cast<std::string*>(value_ptr)).c_str()); ///< Text
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second) ///< If it is an array
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    std::string value_str = *static_cast<std::string*>(value_ptr);
                    if (value_str.find_first_of('/') != std::string::npos)
                    {
                        std::filesystem::path value_path(value_str);
                        if (value_path.is_absolute())
                        {
                            value_path = VKernel::Path::getRelativePath(asset_folder, value_path);
                        }
                        value_str = value_path.generic_string();
                        if (value_str.size() >= 2 && value_str[0] == '.' && value_str[1] == '.')
                        {
                            value_str.clear();
                        }
                    }
                    ImGui::Text("%s", value_str.c_str());
                }
            }
        };
    }

    void DrawVecControl(const std::string& label, VKernel::Vector3& values, float resetValue, float columnWidth)
    {
        // Text
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 {0, 0});

        float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        // X
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 {0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 {0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Y
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.2f, 0.45f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 {0.3f, 0.55f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 {0.2f, 0.45f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Z
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 {0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 {0.1f, 0.25f, 0.8f, 1.0f});
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);
        ImGui::PopID();
    }

    void EditorUI::createClassUI(VKernel::Reflection::ReflectionInstance& instance)
    {
        // Recursively on the base class first
        VKernel::Reflection::ReflectionInstance* reflection_instance;
        int count = instance.m_meta.getBaseClassReflectionInstanceList(reflection_instance, instance.m_instance);
        for (int index = 0; index < count; index++)
        {
            createClassUI(reflection_instance[index]);
        }

        // current class
        createLeafNodeUI(instance);

        if (count > 0)
            delete[] reflection_instance;
    }

    void EditorUI::createLeafNodeUI(VKernel::Reflection::ReflectionInstance& instance)
    {
        VKernel::Reflection::FieldAccessor* fields;
        int                                 fields_count = instance.m_meta.getFieldsList(fields);
        for (size_t index = 0; index < fields_count; index++) ///< Each field of the class
        {
            auto field = fields[index];
            if (field.isArrayType()) ///< If it is an array
            {
                VKernel::Reflection::ArrayAccessor array_accessor;
                if (VKernel::Reflection::TypeMeta::newArrayAccessorFromName(field.getFieldTypeName(), array_accessor))
                {
                    void* field_instance = field.get(instance.m_instance);
                    int   array_count    = array_accessor.getSize(field_instance);
                    m_editor_ui_creator["TreeNodePush"](std::string(field.getFieldName()) + "[" +
                                                            std::to_string(array_count) + "]",
                                                        nullptr); ///< [count]
                    auto item_type_meta_item =
                        VKernel::Reflection::TypeMeta::newMetaFromName(array_accessor.getElementTypeName());
                    auto item_ui_creator_iterator = m_editor_ui_creator.find(item_type_meta_item.getTypeName());
                    for (int index = 0; index < array_count; index++) ///< Iterate through each element
                    {
                        if (item_ui_creator_iterator == m_editor_ui_creator.end()) ///< If not in the map
                        {
                            m_editor_ui_creator["TreeNodePush"]("[" + std::to_string(index) + "]",
                                                                nullptr); ///< [index]
                            auto object_instance =
                                VKernel::Reflection::ReflectionInstance(VKernel::Reflection::TypeMeta::newMetaFromName(
                                                                            item_type_meta_item.getTypeName().c_str()),
                                                                        array_accessor.get(index, field_instance));
                            createClassUI(object_instance); ///< Recursion
                            m_editor_ui_creator["TreeNodePop"]("[" + std::to_string(index) + "]", nullptr);
                        }
                        else
                        {
                            if (item_ui_creator_iterator == m_editor_ui_creator.end())
                            {
                                continue;
                            }
                            m_editor_ui_creator[item_type_meta_item.getTypeName()](
                                "[" + std::to_string(index) + "]",
                                array_accessor.get(index, field_instance)); ///< else push
                        }
                    }
                    m_editor_ui_creator["TreeNodePop"](field.getFieldName(), nullptr);
                }
            }
            auto ui_creator_iterator = m_editor_ui_creator.find(field.getFieldTypeName());
            if (ui_creator_iterator == m_editor_ui_creator.end()) ///< If not in the map
            {
                VKernel::Reflection::TypeMeta field_meta =
                    VKernel::Reflection::TypeMeta::newMetaFromName(field.getFieldTypeName());
                if (field.getTypeMeta(field_meta)) ///< If a class has metadata
                {
                    auto child_instance =
                        VKernel::Reflection::ReflectionInstance(field_meta, field.get(instance.m_instance));
                    m_editor_ui_creator["TreeNodePush"](field_meta.getTypeName(), nullptr);
                    createClassUI(child_instance); ///< Recursion
                    m_editor_ui_creator["TreeNodePop"](field_meta.getTypeName(), nullptr);
                }
                else ///< else continue
                {
                    if (ui_creator_iterator == m_editor_ui_creator.end())
                    {
                        continue;
                    }
                    m_editor_ui_creator[field.getFieldTypeName()](field.getFieldName(), field.get(instance.m_instance));
                }
            }
            else ///< else push
            {
                m_editor_ui_creator[field.getFieldTypeName()](field.getFieldName(), field.get(instance.m_instance));
            }
        }
        delete[] fields;
    }

    void EditorUI::initialize(VKernel::WindowUIInitInfo init_info)
    {
        // set imgui state
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; ///< Enable docking feature
        io.ConfigDockingAlwaysTabBar         = true;      ///< Always show the title bar
        io.ConfigWindowsMoveFromTitleBarOnly = true;      ///< Window can only be dragged from the title bar
        io.IniFilename                       = nullptr;   ///< Do not save the layout
        io.FontGlobalScale                   = 2.0f;

        // set color style
        setUIColorStyle();
    }

    void EditorUI::preRender()
    {
        static bool is_first = true;
        static int  pre_window_x, pre_window_y, pre_window_width, pre_window_height;
        if (!VKernel::g_is_full_screen_mode && VKernel::g_is_editor_mode)
        {
            if (!is_first)
            {
                glfwSetWindowSize(
                    g_editor_global_context.m_window_system->getWindow(), pre_window_width, pre_window_height);
                glfwSetWindowPos(g_editor_global_context.m_window_system->getWindow(), pre_window_x, pre_window_y);
            }
            showEditorUI();
            is_first = true;
        }
        else if (VKernel::g_is_full_screen_mode)
        {
            int window_x, window_y, window_width, window_height;
            glfwGetWindowPos(g_editor_global_context.m_window_system->getWindow(), &window_x, &window_y);
            glfwGetWindowSize(g_editor_global_context.m_window_system->getWindow(), &window_width, &window_height);
            if (is_first)
            {
                pre_window_x      = window_x;
                pre_window_y      = window_y;
                pre_window_width  = window_width;
                pre_window_height = window_height;
                glfwSetWindowSize(
                    g_editor_global_context.m_window_system->getWindow(), window_width, window_height + 35);
                glfwSetWindowPos(g_editor_global_context.m_window_system->getWindow(), window_x, window_y - 35);
                glfwGetWindowPos(g_editor_global_context.m_window_system->getWindow(), &window_x, &window_y);
                glfwGetWindowSize(g_editor_global_context.m_window_system->getWindow(), &window_width, &window_height);
                is_first = false;
            }

            VKernel::Vector2 render_target_window_pos  = VKernel::Vector2(0, 0);
            VKernel::Vector2 render_target_window_size = VKernel::Vector2(window_width, window_height);

            VKernel::g_runtime_global_context.m_render_system->updateEngineContentViewport(render_target_window_pos.x,
                                                                                           render_target_window_pos.y,
                                                                                           render_target_window_size.x,
                                                                                           render_target_window_size.y);

            g_editor_global_context.m_input_manager->setEngineWindowPos(render_target_window_pos);
            g_editor_global_context.m_input_manager->setEngineWindowSize(render_target_window_size);

            showEditorMenu(&m_editor_menu_window_open);
        }
        else
        {
            showEditorMenu(&m_editor_menu_window_open);
            showEditorGameWindow(&m_game_engine_window_open);
        }
    }

    void EditorUI::drawAxisToggleButton(const char* string_id, bool check_state, int axis_mode)
    {
        if (check_state) ///< If this button is selected
        {
            // set id and color
            ImGui::PushID(string_id);
            ImVec4 check_button_color = ImVec4(93.0f / 255.0f, 10.0f / 255.0f, 66.0f / 255.0f, 1.00f);
            ImGui::PushStyleColor(ImGuiCol_Button,
                                  ImVec4(check_button_color.x, check_button_color.y, check_button_color.z, 0.40f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, check_button_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, check_button_color);

            // render button
            ImGui::Button(string_id);

            // pop
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
        else
        {
            if (ImGui::Button(string_id)) ///< render, and When the button is clicked
            {
                check_state = true;
                g_editor_global_context.m_scene_manager->setEditorAxisMode(
                    (EditorAxisMode)axis_mode);                                    ///< update axis mode
                g_editor_global_context.m_scene_manager->drawSelectedEntityAxis(); ///< render gizmo axis
            }
        }
    }

    void EditorUI::buildEditorFileAssetsUITree(EditorFileNode* node)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        // build
        const bool is_folder = (node->m_child_nodes.size() > 0);
        if (is_folder) ///< If it's not a leaf node
        {
            bool open =
                ImGui::TreeNodeEx(node->m_file_name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth); ///< create treenode

            // set node type
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(100.0f);
            ImGui::TextUnformatted(node->m_file_type.c_str());

            // Recursive construction
            if (open) ///< If the node is expanded
            {
                for (int child_n = 0; child_n < node->m_child_nodes.size(); child_n++)
                    buildEditorFileAssetsUITree(node->m_child_nodes[child_n].get());
                ImGui::TreePop();
            }
        }
        else
        {
            ImGui::TreeNodeEx(node->m_file_name.c_str(),
                              ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen |
                                  ImGuiTreeNodeFlags_SpanFullWidth); ///< create treenode

            // If clicked
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                onFileContentItemClicked(node);
            }

            // set node type
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(100.0f);
            ImGui::TextUnformatted(node->m_file_type.c_str());
        }
    }

    void EditorUI::showEditorUI()
    {
        // Render 5 windows
        showEditorMenu(&m_editor_menu_window_open);
        showEditorWorldObjectsWindow(&m_asset_window_open);
        showEditorGameWindow(&m_game_engine_window_open);
        showEditorFileContentWindow(&m_file_content_window_open);
        showEditorDetailWindow(&m_detail_window_open);
    }

    void EditorUI::showEditorMenu(bool* p_open)
    {
        // create menu window

        // Set window position and size to fill the viewport
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(main_viewport->WorkPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(main_viewport->WorkSize, ImGuiCond_Always);
        ImGui::SetNextWindowViewport(main_viewport->ID);

        // create window
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoTitleBar | // No title bar: at the top of the window, shows window name, includes actions
                                          // (minimize, maximize, close)
            ImGuiWindowFlags_NoCollapse | // Cannot be collapsed
            ImGuiWindowFlags_NoResize |   // Cannot be resized
            ImGuiWindowFlags_NoMove |     // Cannot be moved
                                          // No background (transparent)
            ImGuiConfigFlags_NoMouseCursorChange |  // Does not change the mouse cursor
            ImGuiWindowFlags_NoBringToFrontOnFocus; // Does not stay on top when focused

        if (VKernel::g_is_editor_mode && !VKernel::g_is_full_screen_mode)
        {
            window_flags |= ImGuiWindowFlags_MenuBar;
        }

        ImGui::Begin("Editor menu", p_open, window_flags);

        // create dockSapce

        ImGuiID main_docking_id = ImGui::GetID("Main Docking");    ///< get dockSpace id
        if (ImGui::DockBuilderGetNode(main_docking_id) == nullptr) ///< If it does not exist，initial
        {
            // remove old dockSpace
            ImGui::DockBuilderRemoveNode(main_docking_id);

            // add dockSpace node
            ImGuiDockNodeFlags dock_flags = ImGuiDockNodeFlags_DockSpace; ///< flag: Allow other windows to dock here
            ImGui::DockBuilderAddNode(main_docking_id, dock_flags);

            // set position and size
            ImGui::DockBuilderSetNodePos(
                main_docking_id,
                ImVec2(main_viewport->WorkPos.x,
                       main_viewport->WorkPos.y + 18.0f)); ///< Set position: move the Y coordinate down by 18 pixels
                                                           ///< (to make room for the menu bar)
            ImGui::DockBuilderSetNodeSize(main_docking_id,
                                          ImVec2(main_viewport->WorkSize.x,
                                                 main_viewport->WorkSize.y - 18.0f)); // set size

            // Window Dock Preset
            ImGuiID center = main_docking_id;
            ImGuiID left;
            ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.5f, nullptr, &left);

            ImGuiID left_other;
            ImGuiID left_file_content = ImGui::DockBuilderSplitNode(left, ImGuiDir_Down, 0.7f, nullptr, &left_other);

            ImGuiID left_game_engine;
            ImGuiID left_asset =
                ImGui::DockBuilderSplitNode(left_other, ImGuiDir_Left, 0.9f, nullptr, &left_game_engine);

            ImGui::DockBuilderDockWindow("World Objects", left_asset);
            ImGui::DockBuilderDockWindow("Components Details", right);
            ImGui::DockBuilderDockWindow("File Content", left_file_content);
            ImGui::DockBuilderDockWindow("Game Engine", left_game_engine);

            // finish
            ImGui::DockBuilderFinish(main_docking_id);
        }

        ImGui::DockSpace(main_docking_id); ///< render dockSapce

        // MenuBar
        if (VKernel::g_is_editor_mode && !VKernel::g_is_full_screen_mode)
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Menu"))
                {
                    if (ImGui::MenuItem("Reload Current Level"))
                    {
                        VKernel::g_runtime_global_context.m_world_manager->reloadCurrentLevel(); ///< reload level
                        VKernel::g_runtime_global_context.m_render_system
                            ->clearForLevelReloading(); ///< clear and reset
                        g_editor_global_context.m_input_manager->resetCameraSpeed();
                    }
                    if (ImGui::MenuItem("Save Current Level"))
                    {
                        VKernel::g_runtime_global_context.m_world_manager->saveCurrentLevel();
                    }
                    if (ImGui::MenuItem("Exit"))
                    {
                        g_editor_global_context.m_engine_runtime->shutdownEngine();
                        exit(0);
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Window"))
                {
                    ImGui::MenuItem("World Objects", nullptr, &m_asset_window_open);
                    // ImGui::MenuItem("Game", nullptr, &m_game_engine_window_open);
                    ImGui::MenuItem("File Content", nullptr, &m_file_content_window_open);
                    ImGui::MenuItem("Detail", nullptr, &m_detail_window_open);
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
        }

        // close window
        ImGui::End();
    }

    void EditorUI::showEditorWorldObjectsWindow(bool* p_open)
    {
        // If the window is closed
        if (!*p_open)
            return;

        if (m_on_game_window)
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        }

        else
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.002f, 0.0f, 0.0f, 1.0f));
        }

        // create window
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        if (!ImGui::Begin("World Objects", p_open, window_flags))
        {
            ImGui::End();
            ImGui::PopStyleColor();
            return;
        }

        // get current level
        std::shared_ptr<VKernel::Level> current_active_level =
            VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        if (current_active_level == nullptr)
            return;

        // Traverse all objects in the Level
        const VKernel::LevelObjectsMap& all_gobjects = current_active_level->getAllGObjects();
        for (auto& id_object_pair : all_gobjects)
        {
            const VKernel::GObjectID          object_id = id_object_pair.first;
            std::shared_ptr<VKernel::GObject> object    = id_object_pair.second;
            const std::string                 name      = object->getName();
            if (name.size() > 0)
            {
                // create Selectable table
                if (ImGui::Selectable(name.c_str(),
                                      g_editor_global_context.m_scene_manager->getSelectedObjectID() ==
                                          object_id)) ///< if you selectable
                {
                    if (g_editor_global_context.m_scene_manager->getSelectedObjectID() !=
                        object_id) ///< if current selected GO != selected table GO
                    {
                        g_editor_global_context.m_scene_manager->onGObjectSelected(object_id); ///< update selected GO
                    }
                    else
                    {
                        g_editor_global_context.m_scene_manager->onGObjectSelected(
                            VKernel::k_invalid_gobject_id); ///< deselected
                    }
                    break;
                }
            }
        }

        ImGui::End();

        ImGui::PopStyleColor();
    }

    void EditorUI::showEditorFileContentWindow(bool* p_open)
    {
        if (!*p_open)
            return;

        // The sequence of processing and rendering images
        if (m_on_game_window)
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        }

        else
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.002f, 0.0f, 0.0f, 1.0f));
        }

        // create window
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        if (!ImGui::Begin("File Content", p_open, window_flags))
        {
            ImGui::End();
            ImGui::PopStyleColor();
            return;
        }

        // table
        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH |
                                       ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
                                       ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTable("File Content", 2, flags))
        {
            // Divided into two columns: name and type
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            // build file tree
            m_editor_file_service.buildEngineFileTree();

            // build ui tree
            EditorFileNode* editor_root_node = m_editor_file_service.getEditorRootNode(); ///< get root node
            buildEditorFileAssetsUITree(editor_root_node);

            ImGui::EndTable();
        }

        ImGui::End();
        ImGui::PopStyleColor();
    }

    void EditorUI::showEditorGameWindow(bool* p_open)
    {
        if (!*p_open)
            return;

        // begin window
        if (VKernel::g_is_editor_mode)
        {
            ImGui::SetNextWindowBgAlpha(1.0f);
        }
        else
        {
            ImGui::SetNextWindowBgAlpha(0.0f);
        }
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar; ///< Window icon: includes menu bar

        if (!ImGui::Begin("Game Engine", nullptr, window_flags))
        {
            ImGui::End();
            return;
        }

        // Set the switch according to the current axis mode
        static bool trans_button_ckecked  = false;
        static bool rotate_button_ckecked = false;
        static bool scale_button_ckecked  = false;

        switch (g_editor_global_context.m_scene_manager->getEditorAxisMode())
        {
            case EditorAxisMode::TranslateMode:
                trans_button_ckecked  = true;
                rotate_button_ckecked = false;
                scale_button_ckecked  = false;
                break;
            case EditorAxisMode::RotateMode:
                trans_button_ckecked  = false;
                rotate_button_ckecked = true;
                scale_button_ckecked  = false;
                break;
            case EditorAxisMode::ScaleMode:
                trans_button_ckecked  = false;
                rotate_button_ckecked = false;
                scale_button_ckecked  = true;
                break;
            default:
                break;
        }

        // MenuBar
        if (ImGui::BeginMenuBar())
        {
            if (VKernel::g_is_editor_mode && !VKernel::g_is_full_screen_mode)
            {
                // axis mode
                ImGui::Indent(10.f);
                drawAxisToggleButton("Trans", trans_button_ckecked, (int)EditorAxisMode::TranslateMode);
                ImGui::Unindent();

                ImGui::SameLine();

                drawAxisToggleButton("Rotate", rotate_button_ckecked, (int)EditorAxisMode::RotateMode);

                ImGui::SameLine();

                drawAxisToggleButton("Scale", scale_button_ckecked, (int)EditorAxisMode::ScaleMode);

                // editor or game mode
                ImGui::SameLine();
            }

            // Application Interval
            float indent_val = 0.0f;
            float x_scale, y_scale;
            glfwGetWindowContentScale(g_editor_global_context.m_window_system->getWindow(), &x_scale, &y_scale);
            float indent_scale = fmaxf(1.0f, fmaxf(x_scale, y_scale));
            indent_val = g_editor_global_context.m_input_manager->getEngineWindowSize().x - 100.0f * indent_scale;
            ImGui::Indent(indent_val);

            if (VKernel::g_is_editor_mode)
            {
                ImGui::PushID("Editor Mode");
                if (ImGui::Button("Editor Mode")) ///< button
                {
                    // When clicked
                    VKernel::g_is_editor_mode = false; ///< bool
                    g_editor_global_context.m_scene_manager->drawSelectedEntityAxis();
                    g_editor_global_context.m_input_manager->resetEditorCommand();
                }
                ImGui::PopID();
            }
            else
            {
                if (ImGui::Button("Game Mode")) ///< button
                {
                    // When clicked
                    VKernel::g_is_editor_mode = true;
                    g_editor_global_context.m_scene_manager->drawSelectedEntityAxis();
                    VKernel::g_runtime_global_context.m_input_system->resetGameCommand();
                    g_editor_global_context.m_render_system->getRenderCamera()->setMainViewMatrix(
                        g_editor_global_context.m_scene_manager->getEditorCamera()->getViewMatrix());
                }
            }

            // pop
            ImGui::Unindent();
            ImGui::EndMenuBar();
        }
        //
        if (ImGui::IsMouseClicked(0))
        {
            ImGuiWindow* window         = ImGui::GetCurrentWindow();
            ImRect       title_bar_rect = window->TitleBarRect();

            ImVec2 mouse_pos = ImGui::GetMousePos();
            m_on_game_window = title_bar_rect.Contains(mouse_pos);
        }

        // Calculate render area size
        VKernel::Vector2 render_target_window_pos  = {0.0f, 0.0f};
        VKernel::Vector2 render_target_window_size = {0.0f, 0.0f};

        auto menu_bar_rect = ImGui::GetCurrentWindow()->MenuBarRect();

        render_target_window_pos.x  = ImGui::GetWindowPos().x;
        render_target_window_pos.y  = menu_bar_rect.Max.y;
        render_target_window_size.x = ImGui::GetWindowSize().x;
        render_target_window_size.y = (ImGui::GetWindowSize().y + ImGui::GetWindowPos().y) - menu_bar_rect.Max.y;

        VKernel::g_runtime_global_context.m_render_system->updateEngineContentViewport(render_target_window_pos.x,
                                                                                       render_target_window_pos.y,
                                                                                       render_target_window_size.x,
                                                                                       render_target_window_size.y);

        g_editor_global_context.m_input_manager->setEngineWindowPos(render_target_window_pos);
        g_editor_global_context.m_input_manager->setEngineWindowSize(render_target_window_size);

        ImGui::End(); ///< end window
    }

    void EditorUI::showEditorDetailWindow(bool* p_open)
    {
        if (!*p_open)
            return;

        if (m_on_game_window)
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        }

        else
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.002f, 0.0f, 0.0f, 1.0f));
        }

        // begin window
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        if (!ImGui::Begin("Components Details", p_open, window_flags))
        {
            ImGui::End();
            ImGui::PopStyleColor();
            return;
        }

        // get selected object
        std::shared_ptr<VKernel::GObject> selected_object =
            g_editor_global_context.m_scene_manager->getSelectedGObject().lock();
        if (selected_object == nullptr)
        {
            ImGui::End();
            ImGui::PopStyleColor();
            return;
        }

        // get selected object name
        const std::string& name = selected_object->getName();
        static char        cname[128];
        memset(cname, 0, 128);
        memcpy(cname, name.c_str(), name.size());

        // InputText
        ImGui::Text("Name");
        ImGui::SameLine();
        ImGui::InputText("##Name", cname, IM_ARRAYSIZE(cname), ImGuiInputTextFlags_ReadOnly);

        // render Components
        static ImGuiTableFlags flags                      = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings;
        auto&&                 selected_object_components = selected_object->getComponents(); ///< get Components
        for (auto component_ptr : selected_object_components)                                 ///< iterate
        {
            m_editor_ui_creator["TreeNodePush"](("<" + component_ptr.getTypeName() + ">").c_str(),
                                                nullptr); ///< push treenode
            auto object_instance = VKernel::Reflection::ReflectionInstance(
                VKernel::Reflection::TypeMeta::newMetaFromName(component_ptr.getTypeName().c_str()),
                component_ptr.operator->());
            createClassUI(object_instance); ///<
            // render
            m_editor_ui_creator["TreeNodePop"](("<" + component_ptr.getTypeName() + ">").c_str(), nullptr); ///< pop
        }

        ImGui::End();

        ImGui::PopStyleColor();
    }

    void EditorUI::setUIColorStyle()
    {
        ImGuiStyle* style  = &ImGui::GetStyle();
        ImVec4*     colors = style->Colors;

        colors[ImGuiCol_Text]                  = ImVec4(0.4745f, 0.4745f, 0.4745f, 1.00f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border]                = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.047f, 0.047f, 0.047f, 0.5411f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.196f, 0.196f, 0.196f, 0.40f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.294f, 0.294f, 0.294f, 0.67f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.0039f, 0.0039f, 0.0039f, 1.00f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.0039f, 0.0039f, 0.0039f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark]             = ImVec4(93.0f / 255.0f, 10.0f / 255.0f, 66.0f / 255.0f, 1.00f);
        colors[ImGuiCol_SliderGrab]            = colors[ImGuiCol_CheckMark];
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.3647f, 0.0392f, 0.2588f, 0.50f);
        colors[ImGuiCol_Button]                = ImVec4(0.0117f, 0.0117f, 0.0117f, 1.00f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.0235f, 0.0235f, 0.0235f, 1.00f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.0353f, 0.0196f, 0.0235f, 1.00f);
        colors[ImGuiCol_Header]                = ImVec4(0.1137f, 0.0235f, 0.0745f, 0.588f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(5.0f / 255.0f, 5.0f / 255.0f, 5.0f / 255.0f, 1.00f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
        colors[ImGuiCol_Separator]             = ImVec4(0.0f, 0.0f, 0.0f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(45.0f / 255.0f, 7.0f / 255.0f, 26.0f / 255.0f, 1.00f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(45.0f / 255.0f, 7.0f / 255.0f, 26.0f / 255.0f, 1.00f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab]                   = ImVec4(6.0f / 255.0f, 6.0f / 255.0f, 8.0f / 255.0f, 1.00f);
        colors[ImGuiCol_TabHovered]            = ImVec4(45.0f / 255.0f, 7.0f / 255.0f, 26.0f / 255.0f, 150.0f / 255.0f);
        colors[ImGuiCol_TabActive]             = ImVec4(47.0f / 255.0f, 6.0f / 255.0f, 29.0f / 255.0f, 1.0f);
        colors[ImGuiCol_TabUnfocused]          = ImVec4(45.0f / 255.0f, 7.0f / 255.0f, 26.0f / 255.0f, 25.0f / 255.0f);
        colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(6.0f / 255.0f, 6.0f / 255.0f, 8.0f / 255.0f, 200.0f / 255.0f);
        colors[ImGuiCol_DockingPreview]        = ImVec4(47.0f / 255.0f, 6.0f / 255.0f, 29.0f / 255.0f, 0.7f);
        colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(2.0f / 255.0f, 2.0f / 255.0f, 2.0f / 255.0f, 1.0f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 1.00f, 0.0f, 1.00f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 1.00f, 0.0f, 1.00f);
    }

    void EditorUI::onFileContentItemClicked(EditorFileNode* node)
    {
        // Cannot be created if it is not of type object
        if (node->m_file_type != "object" && node->m_file_type != "level")
            return;

        // Find the currently active level
        std::shared_ptr<VKernel::Level> level =
            VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        if (level == nullptr)
            return;

        if (node->m_file_type == "object")
        {
            // create new object
            const unsigned int         new_object_index = ++m_new_object_index_map[node->m_file_name];
            VKernel::ObjectInstanceRes new_object_instance_res;
            new_object_instance_res.m_name = "New_" + VKernel::Path::getFilePureName(node->m_file_name) + "_" +
                                             std::to_string(new_object_index); ///< Set the name of the new object
            new_object_instance_res.m_definition =
                VKernel::g_runtime_global_context.m_asset_manager->getFullPath(node->m_file_path).generic_string();
            size_t new_gobject_id = level->createObject(new_object_instance_res); ///< create new object
            if (new_gobject_id != VKernel::k_invalid_gobject_id)
            {
                g_editor_global_context.m_scene_manager->onGObjectSelected(new_gobject_id); ///< Update Selected Object
            }
        }
        else if (node->m_file_type == "level")
        {
            VKernel::g_runtime_global_context.m_world_manager->loadNewLevel(node->m_file_path); ///< reload level
            VKernel::g_runtime_global_context.m_render_system->clearForLevelReloading();        ///< clear and reset
            g_editor_global_context.m_input_manager->resetCameraSpeed();
        }
    }

    std::string EditorUI::getLeafUINodeParentLabel()
    {
        std::string parent_label;
        int         array_size = g_editor_node_state_array.size();
        for (int index = 0; index < array_size; index++) ///< Loop parent node name
        {
            parent_label += g_editor_node_state_array[index].first + "::"; ///< +=
        }
        return parent_label;
    }
} // namespace ReCoder

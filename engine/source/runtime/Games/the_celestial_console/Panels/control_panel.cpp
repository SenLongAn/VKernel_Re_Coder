#include "runtime/Games/the_celestial_console/Panels/control_panel.h"

#include "runtime/function/character/character.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/object/object.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/resource/config_manager/config_manager.h"
#include "runtime/resource/res_type/components/mesh.h"

#include "control_panel.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Games
{
    WINDOWUI_REGISTER(ControPanel, true);

    std::vector<std::pair<std::string, bool>> g_editor_node_state_array; ///< treeNodeName, Is it expanded
    int                                       g_node_depth = -1;         ///< TreeNode Depth

    void DrawVecControl(const std::string& label,
                        VKernel::Vector3&  values,
                        float              resetValue  = 0.0f,
                        float              columnWidth = 100.0f);

    ControPanel::ControPanel(bool isGameMode)
    {
        m_only_game_mode = isGameMode;

        // TreeNodePush
        m_functions["TreeNodePush"] = [this](const std::string& name, void* value_ptr) -> void { ///< nodeName
            bool node_state = false;
            g_node_depth++;
            if (g_node_depth > 0) ///< If it's not the root node
            {
                if (g_editor_node_state_array[g_node_depth - 1].second) ///< Parent node expanded
                {
                    node_state = ImGui::TreeNodeEx(name.c_str(),
                                                   ImGuiTreeNodeFlags_SpanFullWidth |
                                                       ImGuiTreeNodeFlags_DefaultOpen); ///< Create Node
                }
                else
                {
                    g_editor_node_state_array.emplace_back(std::pair(name.c_str(), node_state));
                    return;
                }
            }
            else ///< If it is a root node
            {
                node_state =
                    ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen);
            }
            g_editor_node_state_array.emplace_back(std::pair(name.c_str(), node_state));
        };

        // TreeNodePop
        m_functions["TreeNodePop"] = [this](const std::string& name, void* value_ptr) -> void {
            if (g_editor_node_state_array[g_node_depth].second) ///< node expanded
            {
                ImGui::TreePop(); ///< pop
            }
            g_editor_node_state_array.pop_back(); ///< pop
            g_node_depth--;
        };

        // Transform
        m_functions["Transform"] = [this](const std::string& name,
                                          void*              value_ptr) -> void { ///< feild name, field instance
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
            }
        };
    }

    void ControPanel::initialize(VKernel::WindowUIInitInfo init_info) {}

    void ControPanel::preRender()
    {
        if (isPanelOpen)
        {
            return;
        }

        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;

        ImGui::SetNextWindowPos(
            ImVec2(viewport.x + viewport.width - viewport.width / 10.0f * 2 + viewport.width / 100.0f, viewport.y),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width / 10.0f * 2, viewport.height), ImGuiCond_Always);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.002f, 0.0f, 0.0f, 1.0f));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

        bool isOpen = true;
        ImGui::Begin("ControPanel", &isOpen, window_flags);

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Control Panel");

        // get selected object
        std::shared_ptr<VKernel::GObject> selected_go = VKernel::g_runtime_global_context.m_render_system->getGO();
        std::shared_ptr<VKernel::Level>   current_level =
            VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<VKernel::Character> current_character = current_level->getCurrentActiveCharacter().lock();

        if (selected_go == nullptr ||
            (current_character != nullptr && current_character->getObject().lock() == selected_go))
        {
            ImGui::End();
            ImGui::PopStyleColor();
            return;
        }

        if (ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered())
        {
            selected_object = false;
        }
        else
        {
            selected_object = true;
        }

        auto&& selected_object_components = selected_go->getComponents(); ///< get Components
        for (auto component_ptr : selected_object_components)             ///< iterate
        {
            std::string str = component_ptr.getTypeName();
            if (str == "TransformComponent")
            {
                m_functions["TreeNodePush"](("<" + component_ptr.getTypeName() + ">").c_str(),
                                            nullptr); ///< push treenode

                auto object_instance = VKernel::Reflection::ReflectionInstance(
                    VKernel::Reflection::TypeMeta::newMetaFromName(component_ptr.getTypeName().c_str()),
                    component_ptr.operator->());

                createLeafNodeUI(object_instance);

                m_functions["TreeNodePop"](("<" + component_ptr.getTypeName() + ">").c_str(), nullptr); ///< pop
            }
            if (str == "MeshComponent")
            {
                m_functions["TreeNodePush"](("<" + component_ptr.getTypeName() + ">").c_str(),
                                            nullptr); ///< push treenode

                auto object_instance = VKernel::Reflection::ReflectionInstance(
                    VKernel::Reflection::TypeMeta::newMetaFromName(component_ptr.getTypeName().c_str()),
                    component_ptr.operator->());
                VKernel::Reflection::FieldAccessor* fields;
                int                                 fields_count = object_instance.m_meta.getFieldsList(fields);
                for (size_t index = 0; index < fields_count; index++)
                {
                    auto field = fields[index];

                    VKernel::MeshComponentRes* mesh_com_ptr =
                        static_cast<VKernel::MeshComponentRes*>(field.get(object_instance.m_instance));
                    VKernel::Vector3& color = mesh_com_ptr->m_color;

                    float myColor[3] = {color.x, color.y, color.z};
                    ImGui::ColorPicker3("Color", myColor);
                    color.x = myColor[0];
                    color.y = myColor[1];
                    color.z = myColor[2];

                    bool& apply_lighting = mesh_com_ptr->m_apply_lighting;
                    ImGui::Checkbox("Apply Lighting", &apply_lighting);
                    bool& apply_texture = mesh_com_ptr->m_apply_texture;
                    ImGui::Checkbox("Apply Texture", &apply_texture);
                }

                m_functions["TreeNodePop"](("<" + component_ptr.getTypeName() + ">").c_str(), nullptr); ///< pop
            }
        }

        ImGui::End();

        ImGui::PopStyleColor();
    }

    void ControPanel::createLeafNodeUI(VKernel::Reflection::ReflectionInstance& instance)
    {
        VKernel::Reflection::FieldAccessor* fields;
        int                                 fields_count = instance.m_meta.getFieldsList(fields);
        for (size_t index = 0; index < fields_count; index++) ///< Each field of the class
        {
            auto field               = fields[index];
            auto ui_creator_iterator = m_functions.find(field.getFieldTypeName());
            if (ui_creator_iterator == m_functions.end()) ///< If not in the map
            {
                VKernel::Reflection::TypeMeta field_meta =
                    VKernel::Reflection::TypeMeta::newMetaFromName(field.getFieldTypeName());
                if (field.getTypeMeta(field_meta)) ///< If a class has metadata
                {
                    auto child_instance =
                        VKernel::Reflection::ReflectionInstance(field_meta, field.get(instance.m_instance));
                    m_functions["TreeNodePush"](field_meta.getTypeName(), nullptr);
                    createLeafNodeUI(child_instance); ///< Recursion
                    m_functions["TreeNodePop"](field_meta.getTypeName(), nullptr);
                }
            }
            else ///< else push
            {
                m_functions[field.getFieldTypeName()](field.getFieldName(), field.get(instance.m_instance));
            }
        }
        delete[] fields;
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
} // namespace Games
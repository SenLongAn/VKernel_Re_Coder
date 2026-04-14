#include "runtime/Games/the_celestial_console/Panels/material_library_panel.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/Games/the_celestial_console/Panels/main_panel.h"
#include "runtime/core/base/macro.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/framework/component/mesh/mesh_component.h"

namespace Games
{
    WINDOWUI_REGISTER(MaterialLibraryPanel, true);

    void MaterialLibraryPanel::initialize(VKernel::WindowUIInitInfo init_info)
    {
        m_mesh_type["GUN"] = 0;
        m_mesh_type["BUILDING"] = 1;
        m_mesh_type["CARRIER AIRCRAFT"] = 2;
        m_mesh_type["DRONE"] = 3;
        m_mesh_type["BOAT"] = 4;

        images.resize(5);
        image_views.resize(5);
        image_allocations.resize(5);
        descriptorSetLayouts.resize(5);
        descriptorSets.resize(5);
        texture_ids.resize(5);
    }

    void MaterialLibraryPanel::preUpdate()
    {
        static bool isFirst = true;
        if (isFirst)
        {
            createImGuiDescriptorSetForTexture(images[0], image_views[0], image_allocations[0], descriptorSetLayouts[0], descriptorSets[0], texture_ids[0],
                                               "asset/texture/icon/gun.jpg", true);
            createImGuiDescriptorSetForTexture(images[1], image_views[1], image_allocations[1], descriptorSetLayouts[1], descriptorSets[1], texture_ids[1],
                                               "asset/texture/icon/Building.jpg", true);
            createImGuiDescriptorSetForTexture(images[2], image_views[2], image_allocations[2], descriptorSetLayouts[2], descriptorSets[2], texture_ids[2],
                                               "asset/texture/icon/carrier_aircraft.jpg", true);
            createImGuiDescriptorSetForTexture(images[3], image_views[3], image_allocations[3], descriptorSetLayouts[3], descriptorSets[3], texture_ids[3],
                                               "asset/texture/icon/drone.jpg", true);
            createImGuiDescriptorSetForTexture(images[4], image_views[4], image_allocations[4], descriptorSetLayouts[4], descriptorSets[4], texture_ids[4],
                                               "asset/texture/icon/boat.jpg", true);
            isFirst = false;
        }
    }

    void MaterialLibraryPanel::preRender()
    {
        // Is it render

        if (!MainPanel::isPanelOpen)
        {
            return;
        }

        if (!MainPanel::isPanelOne)
        {
            return;
        }

        // window
        VkViewport viewport =
            VKernel::g_runtime_global_context.m_render_system->getVulkanAPI()->getSwapchainInfo().viewport;

        ImGui::SetNextWindowPos(
            ImVec2(viewport.x, viewport.y),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(viewport.width / 10.0f * 2, viewport.height), ImGuiCond_Always);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;

        ImGui::Begin(ICON_FA_ANCHOR " MATERIAL LIBRARY", nullptr, window_flags);

        ImGui::GetStyle().WindowPadding = ImVec2(0, 0);

        float window_x = ImGui::GetWindowSize().x;

        // Automatically adjust font size
        float windowArea = ImGui::GetWindowSize().x * ImGui::GetWindowSize().y;
        float scale = sqrtf(windowArea / (640 * 0.20 * 400)) * 0.5;
        ImGui::SetWindowFontScale(scale);

        // Whether the panel can be penetrated
        if (isMouseInWindowRange())
        {
            selected_object = false;
        }

        // text
        static std::string name = "";
        static std::string introduction = "";
        addSeparator();
        ImGui::TextWrapped((std::string(ICON_FA_CALCULATOR) + " " + name).c_str());
        addSeparator();
        ImGui::TextWrapped((std::string(ICON_FA_CALCULATOR) + " " + introduction).c_str());

        // Scrollable window
        addSeparator();
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 4));

        ImGui::BeginChild("Materials", ImVec2(ImGui::GetWindowSize().x + ImGui::GetWindowSize().x * 0.05, (ImGui::GetWindowSize().y / 4) * 2.8), window_flags);

        // Whether the panel can be penetrated
        if (isMouseInWindowRange())
        {
            selected_object = false;
        }

        // button
        int col = 3;
        float spacing = window_x * 0.035;
        ImVec2 buttonSize((window_x - spacing) / col, (window_x - spacing) / col);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((spacing / 6.0f), (spacing / 6.0f)));

        // get current level
        std::shared_ptr<VKernel::Level> current_active_level =
            VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        if (current_active_level == nullptr)
            return;

        // Traverse all objects in the Level
        std::unordered_set<std::string> visiable_type;
        const VKernel::LevelObjectsMap &all_gobjects = current_active_level->getAllGObjects();
        int i = 0;
        for (auto &id_object_pair : all_gobjects)
        {
            VKernel::MeshComponent *mesh_component =
                id_object_pair.second->tryGetComponent(VKernel::MeshComponent, "MeshComponent");
            std::string cur_name = mesh_component->getMeshComponentRes().m_name;
            std::string cur_type = mesh_component->getMeshComponentRes().m_type;
            if (cur_type != "" && !visiable_type.count(cur_name))
            {
                if ((i % col) != 0)
                    ImGui::SameLine(0, 0);

                ImGui::PushID(id_object_pair.second.get());
                if (ImGui::ImageButton(texture_ids[m_mesh_type[cur_type]], buttonSize))
                {
                    name = cur_name;
                    introduction = mesh_component->getMeshComponentRes().m_introduction;
                }
                ImGui::PopID();

                i++;
                visiable_type.insert(cur_name);
            }
        }
        ImGui::PopStyleVar(3);

        ImGui::EndChild();

        ImGui::End();
    }
} // namespace Games
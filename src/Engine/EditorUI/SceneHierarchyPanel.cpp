#include "EditorUI/SceneHierarchyPanel.h"
#include "components/components.h"

#include <rlImGui.h>
#include <functional>

namespace BloxEngine::EditorUI
{
    // TODO: Change to BaseEntity
    void SceneHierarchyPanel::SetSelectedEntity(entt::entity &entity)
    {
        m_selectedEntity = BaseEntity(entity, m_ptrScenePanel);
    }

    template <typename T>
    void SceneHierarchyPanel::DrawAddComponentEntry(const std::string &entryName)
    {
        if (!m_selectedEntity.HasComponent<T>())
        {
            if (ImGui::MenuItem(entryName.c_str()))
            {
                m_selectedEntity.AddComponent<T>();
                ImGui::CloseCurrentPopup();
            }
        }
    }
    template <typename T>
    void SceneHierarchyPanel::DrawComponentInfo(std::function<void(T &)> drawFunction, const std::string &componentName)
    {
        if (m_selectedEntity.HasComponent<T>())
        {
            if (ImGui::BeginChild(componentName.c_str(), ImVec2(0, 0), true))
            {
                ImGui::SeparatorText(componentName.c_str());
                auto &component = m_selectedEntity.GetComponent<T>();
                drawFunction(component);
            }
            ImGui::EndChild();
        }
    }

    void SceneHierarchyPanel::DrawComponents(BaseEntity &entity)
    {
        // Draw the add component menu
        if (ImGui::BeginMenu("Add Component"))
        {
            DrawAddComponentEntry<TagComponent>("Tag");
            DrawAddComponentEntry<TransformComponent>("Transform");
            DrawAddComponentEntry<ModelComponent>("Model");
            DrawAddComponentEntry<ModelMaterialComponent>("Model material");
            DrawAddComponentEntry<TerrainComponent>("Terrain");
            ImGui::EndMenu();
        }

        DrawComponentInfo<TagComponent>([](TagComponent &tag)
        { 
            ImGui::Text("Tag: %s", tag.Tag.c_str());
        }, "Tag");

        DrawComponentInfo<TransformComponent>([](TransformComponent &transform)
        {
            ImGui::DragFloat3("Translation", &transform.transform.translation.x, 0.1f);
            ImGui::DragFloat3("Rotation", &transform.transform.rotation.x, 0.1f);
            ImGui::DragFloat3("Scale", &transform.transform.scale.x, 0.1f); 
        }, "Transform");
        
        DrawComponentInfo<ModelComponent>([&entity](ModelComponent &model)
        {
            ImGui::Text("Meshes: %i", model.ModelObject.meshCount);
            // TODO: Make a load button
            // TODO: make it say which model is loaded
            // TODO: FOR THE LOVE OF GOD, REMOVE MODELMATERIALCOMPONENT

            if(entity.HasComponent<ModelMaterialComponent>())
            {
                if (ImGui::CollapsingHeader("Albedo"))
                {
                    rlImGuiImage(&model.ModelObject.materials[0].maps[MATERIAL_MAP_ALBEDO].texture);
                }

                if (ImGui::CollapsingHeader("Normal"))
                {
                    rlImGuiImage(&model.ModelObject.materials[0].maps[MATERIAL_MAP_NORMAL].texture);
                }

                if (ImGui::CollapsingHeader("Metalness"))
                {
                    rlImGuiImage(&model.ModelObject.materials[0].maps[MATERIAL_MAP_METALNESS].texture);
                }

                if (ImGui::CollapsingHeader("Roughness"))
                {
                    rlImGuiImage(&model.ModelObject.materials[0].maps[MATERIAL_MAP_ROUGHNESS].texture);
                }

                if (ImGui::CollapsingHeader("Occlusion"))
                {
                    rlImGuiImage(&model.ModelObject.materials[0].maps[MATERIAL_MAP_OCCLUSION].texture);
                }
            }
        }, "Model");
    }

    void SceneHierarchyPanel::Draw()
    {
        // TODO: Change it so that every entity in a scene is drawn, currently only entities with a TagComponent are drawn
        ImGui::Begin("Scene Hierarchy");
        for (auto entity : m_ptrScenePanel->m_Registry.view<TagComponent>())
        {
            auto &tag = m_ptrScenePanel->m_Registry.get<TagComponent>(entity);

            // If this entity is being renamed
            // TODO: Fix crash when renaming an empty string
            if (entityBeingRenamed == entity)
            {
                if (ImGui::InputText("##RenameEntity", renameBuffer, sizeof(renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    // Commit the rename when the user presses Enter
                    tag.Tag = renameBuffer;
                    entityBeingRenamed = entt::null; // Clear rename state
                }

                // Stop renaming if user clicks outside or presses escape
                if (!ImGui::IsItemActive() && !ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
                {
                    entityBeingRenamed = entt::null;
                }
            }
            else
            {
                // Display the tag normally
                if (ImGui::Selectable(tag.Tag.c_str(), m_selectedEntity == entity))
                {
                    SetSelectedEntity(entity);
                }

                // Start renaming on double-click
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    entityBeingRenamed = entity;
                    strncpy(renameBuffer, tag.Tag.c_str(), sizeof(renameBuffer) - 1);
                    renameBuffer[sizeof(renameBuffer) - 1] = '\0'; // Ensure null-termination
                }
            }

            // Right-click context menu
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete"))
                {
                    m_ptrScenePanel->DestroyEntity(m_selectedEntity);
                    if (m_selectedEntity == entity)
                    {
                        m_selectedEntity = BaseEntity(entt::null, m_ptrScenePanel);
                    }
                }

                ImGui::EndPopup();
            }

            if(ImGui::BeginPopupContextWindow())
            {
                if(ImGui::MenuItem("Create Entity"))
                {
                    m_ptrScenePanel->CreateEntity("Entity");
                }
                ImGui::EndPopup();
            }
        }
        ImGui::End();

        ImGui::Begin("Properties");
        if (m_selectedEntity)
        {
            DrawComponents(m_selectedEntity);
        }
        ImGui::End();
    }
} // namespace BloxEngine::EditorUI
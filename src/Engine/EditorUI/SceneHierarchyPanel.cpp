#include "components/components.h"
#include "EditorUI/SceneHierarchyPanel.h"


namespace BloxEngine::EditorUI
{
    void SceneHierarchyPanel::SetSelectedEntity(entt::entity& entity)
    {
        m_selectedEntity = entity;
    }

    void SceneHierarchyPanel::Draw()
    {
        // TODO: Change to modal
        // TODO: Change it so that every entity in a scene is drawn, currently only entities with a TagComponent are drawn
        ImGui::Begin("Scene Hierarchy");
        for (auto entity : m_ptrScenePanel->m_Registry.view<TagComponent>())
        {
            auto &tag = m_ptrScenePanel->m_Registry.get<TagComponent>(entity);
            if(ImGui::Selectable(tag.Tag.c_str(), m_selectedEntity == entity))
            {
                SetSelectedEntity(entity);
            }
        }
        ImGui::End();
    }
} // namespace BloxEngine::EditorUI
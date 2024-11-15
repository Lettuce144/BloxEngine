#ifndef SCENE_HIERARCHY_PANEL_H
#define SCENE_HIERARCHY_PANEL_H
#pragma once

#include "EditorUI/EditorPanel.h"
#include "scene.h"

#include <entt/entt.hpp>

namespace BloxEngine::EditorUI
{

    class SceneHierarchyPanel : public EditorPanel
    {
    public:
        SceneHierarchyPanel(std::shared_ptr<Scene> activeScene) : m_ptrScenePanel(activeScene) {};
        ~SceneHierarchyPanel() {};

        void Draw() override;
        void SetSelectedEntity(entt::entity& entity);

    private:
        // Pointer to scene to display a list of its entities
        std::shared_ptr<Scene> m_ptrScenePanel;

        // TODO: Implement selection of entities
        entt::entity m_selectedEntity;
    };

} // namespace BloxEngine

#endif // SCENE_HIERARCHY_PANEL_H
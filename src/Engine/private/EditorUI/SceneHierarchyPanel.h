#ifndef SCENE_HIERARCHY_PANEL_H
#define SCENE_HIERARCHY_PANEL_H
#pragma once

#include "EditorUI/EditorPanel.h"
#include "scene.h"
#include "baseentity.hpp"

#include <entt/entt.hpp>

namespace BloxEngine::EditorUI
{

    class SceneHierarchyPanel : public EditorPanel
    {
    public:
        SceneHierarchyPanel(std::shared_ptr<Scene> activeScene) : m_ptrScenePanel(activeScene) {};
        ~SceneHierarchyPanel() {};

        void Draw() override;
        void SetSelectedEntity(entt::entity &entity);

        BaseEntity GetSelectedEntity() const { m_selectedEntity; }
        // Private methods
    private:
        template <typename T>
        void DrawAddComponentEntry(const std::string &entryName);
        
        template <typename T>
        // Draw a component's info in a child window
        void DrawComponentInfo(std::function<void(T &)> drawFunction, const std::string &componentName);
        

        void DrawComponents(BaseEntity &entity);
    private:
        // Pointer to scene to display a list of its entities
        std::shared_ptr<Scene> m_ptrScenePanel;

        BaseEntity m_selectedEntity = BaseEntity(entt::null, m_ptrScenePanel);

        // State to track the entity being renamed
        entt::entity entityBeingRenamed = entt::null;
        char renameBuffer[256] = "";
    };

} // namespace BloxEngine

#endif // SCENE_HIERARCHY_PANEL_H
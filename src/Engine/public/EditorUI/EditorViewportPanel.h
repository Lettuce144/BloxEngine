#ifndef EDITOR_VIEWPORT_PANEL_H
#define EDITOR_VIEWPORT_PANEL_H
#pragma once

#include "EditorPanel.h"
#include "scene.h"

#include "Application/gameinstance.h"
#include "EditorUI/SceneHierarchyPanel.h"

#include <ImGuizmo.h>
#include <raylib.h>
#include <memory>

namespace BloxEngine::EditorUI
{

    class EditorViewportPanel : public EditorPanel
    {
    public:
        EditorViewportPanel(Camera3D &camera, GameInstance *gameInstance, std::shared_ptr<SceneHierarchyPanel> sceneHierarchyPanel)
            : m_refCamera(camera),
              m_ptrGameInstance(gameInstance),
              m_ptrSceneHierarchyPanel(sceneHierarchyPanel)
        {
            m_renderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
            m_Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
        };

        ~EditorViewportPanel()
        {
            UnloadRenderTexture(m_renderTexture);
        };

        // Draw the panel
        void Draw() override;
        void UpdateViewport();

        bool IsFocused() const { return m_Focused; }

    private:
        bool m_Focused;

        Camera3D &m_refCamera;
        GameInstance *m_ptrGameInstance;
        RenderTexture2D m_renderTexture;

        std::shared_ptr<SceneHierarchyPanel> m_ptrSceneHierarchyPanel; // Store the SceneHierarchyPanel
    };

} // namespace BloxEngine::EditorUI

#endif // EDITOR_VIEWPORT_PANEL_H
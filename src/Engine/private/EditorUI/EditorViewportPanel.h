#ifndef EDITOR_VIEWPORT_PANEL_H
#define EDITOR_VIEWPORT_PANEL_H
#pragma once

#include "EditorPanel.h"
#include "scene.h"

#include <raylib.h>

namespace BloxEngine::EditorUI
{

    class EditorViewportPanel : public EditorPanel
    {
    public:
        EditorViewportPanel(Camera3D& camera, std::shared_ptr<Scene> activeScene) : m_refCamera(camera), m_ptrActiveScene(activeScene)
        {
            m_renderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
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
        bool m_Focused = false;

        Camera3D& m_refCamera;
        std::shared_ptr<Scene> m_ptrActiveScene;
        RenderTexture2D m_renderTexture;
    };

} // namespace BloxEngine::EditorUI

#endif // EDITOR_VIEWPORT_PANEL_H
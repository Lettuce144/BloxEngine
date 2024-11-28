#include "EditorUI/EditorViewportPanel.h"

#include <rlImGui.h>

namespace BloxEngine::EditorUI
{
    void EditorViewportPanel::Draw()
    {
        if (ImGui::Begin("3D View", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            m_Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);

            if (IsKeyPressed(KEY_ESCAPE))
                m_Focused = false;

            ImVec2 panelSize = ImGui::GetContentRegionAvail();
            if (panelSize.x != m_renderTexture.texture.width || panelSize.y != m_renderTexture.texture.height)
            {
                UnloadRenderTexture(m_renderTexture);
                m_renderTexture = LoadRenderTexture(static_cast<int>(panelSize.x), static_cast<int>(panelSize.y));
            }

            rlImGuiImageRenderTextureFit(&m_renderTexture, false);
        }
        ImGui::End();
    }

    void EditorViewportPanel::UpdateViewport()
    {
        // Resize render texture if necessary
        // int newWidth = GetScreenWidth();
        // int newHeight = GetScreenHeight();
        // if (newWidth != m_renderTexture.texture.width || newHeight != m_renderTexture.texture.height)
        // {
        //     UnloadRenderTexture(m_renderTexture);
        //     m_renderTexture = LoadRenderTexture(newWidth, newHeight);
        // }

        BeginTextureMode(m_renderTexture);
        {
            ClearBackground(SKYBLUE);

            BeginMode3D(m_refCamera);
            {
                m_ptrActiveScene->OnUpdate();
                DrawPlane(Vector3{0.0f, 0.0f, 0.0f}, Vector2{10.0f, 10.0f}, LIGHTGRAY);
            }
            EndMode3D();
        }
        EndTextureMode();
    }
} // namespace BloxEngine::EditorUI
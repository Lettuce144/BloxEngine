#include "EditorUI/EditorViewportPanel.h"
#include "ImGuizmo.h"
#include "components/components.h"
#include "raylib.h"

#include <rlImGui.h>

namespace BloxEngine::EditorUI
{

    Matrix MatrixFromFloat16(const float *f)
    {
        Matrix mat;
        mat.m0 = f[0];
        mat.m4 = f[4];
        mat.m8 = f[8];
        mat.m12 = f[12];
        mat.m1 = f[1];
        mat.m5 = f[5];
        mat.m9 = f[9];
        mat.m13 = f[13];
        mat.m2 = f[2];
        mat.m6 = f[6];
        mat.m10 = f[10];
        mat.m14 = f[14];
        mat.m3 = f[3];
        mat.m7 = f[7];
        mat.m11 = f[11];
        mat.m15 = f[15];
        return mat;
    }

    void EditorViewportPanel::Draw()
    {

        if (ImGui::Begin("3D View", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                // if (ImGui::BeginMenu("Debug", "F12"))
                // {
                //     if (ImGui::MenuItem("Frame debug"))
                //     {
                //         // RendererDebug::DebugFrame(GameInstance *ptr)
                //     }
                //     ImGui::EndMenu();
                if(ImGui::Button("Focus (Esc)"))
                {
                    m_Focused = !m_Focused; 
                }
                ImGui::EndMenuBar();
            }

            if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && IsKeyPressed(KEY_ESCAPE))
            {
                m_Focused = !m_Focused;
            }

            ImVec2 panelSize = ImGui::GetContentRegionAvail();
            if (panelSize.x != m_renderTexture.texture.width || panelSize.y != m_renderTexture.texture.height)
            {
                UnloadRenderTexture(m_renderTexture);
                m_renderTexture = LoadRenderTexture(static_cast<int>(panelSize.x), static_cast<int>(panelSize.y));
            }

            rlImGuiImageRenderTextureFit(&m_renderTexture, false);

            // ImGuizmo setup
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
            ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

            ImVec2 contentPos = ImVec2(windowPos.x + contentMin.x, windowPos.y + contentMin.y);
            ImVec2 contentSize = ImVec2(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

            ImGuizmo::SetRect(contentPos.x, contentPos.y, contentSize.x, contentSize.y);

            // View matrix
            Matrix view = GetCameraMatrix(m_refCamera);
            const float *viewMatrix = MatrixToFloat(view);

            // Projection matrix
            float aspect = panelSize.x / panelSize.y;
            Matrix projection = MatrixPerspective(DEG2RAD * m_refCamera.fovy, aspect, rlGetCullDistanceNear(), rlGetCullDistanceFar());
            const float *projectionMatrix = MatrixToFloat(projection);

            if (m_ptrSceneHierarchyPanel->GetSelectedEntity())
            {
                ImGuizmo::Enable(true);

                static ImGuizmo::OPERATION currentGizmoOperation = ImGuizmo::TRANSLATE;
                static ImGuizmo::MODE currentGizmoMode = ImGuizmo::WORLD;

                // TODO: Implement a input system for the engine

                if (IsKeyPressed(KEY_R))
                    currentGizmoOperation = ImGuizmo::ROTATE;
                if (IsKeyPressed(KEY_T))
                    currentGizmoOperation = ImGuizmo::TRANSLATE;
                if (IsKeyPressed(KEY_Y))
                    currentGizmoOperation = ImGuizmo::SCALE;

                auto &transformComponent = m_ptrSceneHierarchyPanel->GetSelectedEntity().GetComponent<TransformComponent>();

                // Create transformation matrix using quaternion instead of Euler angles
                Quaternion rotationQuat = QuaternionFromEuler(
                    transformComponent.rotation.x * DEG2RAD,
                    transformComponent.rotation.y * DEG2RAD,
                    transformComponent.rotation.z * DEG2RAD);
                Matrix rotationMat = QuaternionToMatrix(rotationQuat);

                Matrix translationMat = MatrixTranslate(transformComponent.translation.x, transformComponent.translation.y, transformComponent.translation.z);
                Matrix scaleMat = MatrixScale(transformComponent.scale.x, transformComponent.scale.y, transformComponent.scale.z);

                Matrix transformMat = MatrixMultiply(MatrixMultiply(scaleMat, rotationMat), translationMat);

                // Convert to float[16] for ImGuizmo
                float transform[16];
                memcpy(transform, MatrixToFloat(transformMat), sizeof(float) * 16);

                ImGuizmo::Manipulate(viewMatrix, projectionMatrix, currentGizmoOperation, currentGizmoMode, transform);

                if (ImGuizmo::IsUsing())
                {
                    Matrix newMat = MatrixFromFloat16(transform);

                    // Decompose matrix into translation, rotation, scale
                    Vector3 translation, rotation, scale;

                    // Extract translation
                    translation = {newMat.m12, newMat.m13, newMat.m14};

                    // Extract scale
                    scale.x = Vector3Length({newMat.m0, newMat.m1, newMat.m2});
                    scale.y = Vector3Length({newMat.m4, newMat.m5, newMat.m6});
                    scale.z = Vector3Length({newMat.m8, newMat.m9, newMat.m10});

                    // Get rotation using quaternion to avoid gimbal lock
                    Matrix rotMat = newMat;
                    rotMat.m0 /= scale.x;
                    rotMat.m1 /= scale.x;
                    rotMat.m2 /= scale.x;
                    rotMat.m4 /= scale.y;
                    rotMat.m5 /= scale.y;
                    rotMat.m6 /= scale.y;
                    rotMat.m8 /= scale.z;
                    rotMat.m9 /= scale.z;
                    rotMat.m10 /= scale.z;

                    Quaternion quat = QuaternionFromMatrix(rotMat);
                    rotation = QuaternionToEuler(quat);
                    rotation = Vector3Scale(rotation, RAD2DEG);

                    // Normalize rotation angles to -180 to 180 range
                    rotation.x = fmodf(rotation.x + 180.0f, 360.0f) - 180.0f;
                    rotation.y = fmodf(rotation.y + 180.0f, 360.0f) - 180.0f;
                    rotation.z = fmodf(rotation.z + 180.0f, 360.0f) - 180.0f;

                    // Update component
                    transformComponent.translation = translation;
                    transformComponent.rotation = rotation;
                    transformComponent.scale = scale;
                }
            }
            else
            {
                ImGuizmo::Enable(false);
            }
        }
        ImGui::End();
    }

    void EditorViewportPanel::UpdateViewport()
    {
        BeginTextureMode(m_renderTexture);
        {
            ClearBackground(BLACK);

            BeginMode3D(m_refCamera);
            {
                m_ptrGameInstance->GetActiveScene()->RenderEditorScene(m_refCamera);
            }
            EndMode3D();
        }
        EndTextureMode();
        // End the geometry pass
    }
} // namespace BloxEngine::EditorUI

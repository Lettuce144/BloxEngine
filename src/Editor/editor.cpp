#include <Application/entrypoint.hpp>
#include <Application/gameinstance.h>

// #include <Application/instanceprops.h>

#include <EditorUI/EditorViewportPanel.h>
#include <EditorUI/SceneHierarchyPanel.h>

#include <memory>
#include <raylib-cpp.hpp>

#include <imgui.h>
#include <rlImGui.h>

// Normally you would create a game when inheriting from GameInstance
// Making this technically a game, however that is how the engine is designed
class Editor : public BloxEngine::GameInstance
{
public:
    Editor(const int &width, const int &height, const char *title)
        : BloxEngine::GameInstance(width, height, title)
    {
    }

    void OnUpdate() override;
    void OnAttach() override;
    void OnDetach() override;

    ~Editor() = default;

private:
    // TODO: Use the editor camera class provided by the engine
    raylib::Camera3D m_editorCamera;

    // Panels
    std::shared_ptr<BloxEngine::EditorUI::SceneHierarchyPanel> m_sceneHierarchyPanel;
    std::shared_ptr<BloxEngine::EditorUI::EditorViewportPanel> m_viewportPanel;
};

BloxEngine::GameInstance *BloxEngine::CreateGameInstance()
{
    return new Editor(1360, 768, "Blox Engine Editor");
}

void Editor::OnAttach()
{
    m_editorCamera = raylib::Camera3D(raylib::Vector3{0.0f, 10.0f, 10.0f},
                                      raylib::Vector3{0.0f, 0.0f, 0.0f},
                                      raylib::Vector3{0.0f, 1.0f, 0.0f},
                                      45.0f, CameraProjection::CAMERA_PERSPECTIVE);

    // --------------------------------------------------
    // Gui setup
    // --------------------------------------------------

    // Initialize rlImGui
    rlImGuiBeginInitImGui();
    {
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;                         // Enable docking
        io.Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Medium.ttf", 16.0f); // TODO: Fix loading fonts, currently look rugged and ugly
    }
    rlImGuiEndInitImGui();

    // Editor viewport panel
    m_viewportPanel = std::make_shared<BloxEngine::EditorUI::EditorViewportPanel>(m_editorCamera, GetActiveScene());
    m_sceneHierarchyPanel = std::make_shared<BloxEngine::EditorUI::SceneHierarchyPanel>(GetActiveScene());
}

void Editor::OnDetach()
{
    rlImGuiShutdown();
}

void Editor::OnUpdate()
{
    // Update the camera
    if (m_viewportPanel->IsFocused())
    {
        m_editorCamera.Update(CAMERA_FREE);
    }

    GetInstanceWindow()->BeginDrawing();
    {
        rlImGuiBegin();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

        // Draw the scene hierarchy panel
        m_sceneHierarchyPanel->Draw();

        // Draw the viewport panel
        m_viewportPanel->Draw();

        // Update the viewport
        m_viewportPanel->UpdateViewport();

        ImGui::PopFont();
        rlImGuiEnd();
    }
    GetInstanceWindow()->EndDrawing();
}

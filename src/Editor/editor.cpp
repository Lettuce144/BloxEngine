#include <Application/entrypoint.hpp>
#include "editor.hpp"

BloxEngine::GameInstance *BloxEngine::CreateGameInstance()
{
  InstanceProperties applicationProperties;

  applicationProperties.appName = "Blox Engine Editor";
  applicationProperties.width = 1360; // Default width
  applicationProperties.height = 768; // Default height
  applicationProperties.fullscreen = false;
  applicationProperties.clearcolor = RAYWHITE;

  return new Editor(applicationProperties);
}

void Editor::OnAttach()
{

  // --------------------------------------------------
  // Gui setup
  // --------------------------------------------------

  // Initialize rlImGui
  rlImGuiBeginInitImGui();
  {
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
    io.Fonts->AddFontFromFileTTF(
        "resources/fonts/Roboto-Medium.ttf",
        16.0f); // TODO: Fix loading fonts, currently look rugged and ugly
    imgui_green_theme();
  }
  rlImGuiEndInitImGui();

  // Initialize the editor camera first
  m_editorCamera = BloxEngine::EditorCamera();

  m_sceneHierarchyPanel =
      std::make_shared<BloxEngine::EditorUI::SceneHierarchyPanel>(
          GetActiveScene());

  // Editor viewport panel
  m_viewportPanel = std::make_shared<BloxEngine::EditorUI::EditorViewportPanel>(
      m_editorCamera, this, m_sceneHierarchyPanel); // Pass SceneHierarchyPanel

  // Set the viewport panel in the editor camera
  m_editorCamera.SetViewportPanel(m_viewportPanel);

  m_menuBar = std::make_unique<EditorMenuBar>(*this);
}

void Editor::OnDetach() { rlImGuiShutdown(); }

void Editor::OnUpdate()
{
  GetInstanceWindow()->BeginDrawing();
  {
    GetInstanceWindow()->ClearBackground(BLANK); // TODO: Make this a setting

    rlImGuiBegin();
    ImGuizmo::BeginFrame();
    
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::DockSpaceOverViewport(0, NULL,
                                 ImGuiDockNodeFlags_PassthruCentralNode);

    // Draw the viewport panel
    m_viewportPanel->UpdateViewport();

    // Update the viewport
    m_viewportPanel->Draw();

    // Draw the scene hierarchy panel
    m_sceneHierarchyPanel->Draw();

    // Draw the menu bar
    m_menuBar->DrawElement();

    // Update the camera
    m_editorCamera.DrawCamera();

    ImGui::PopFont();
    rlImGuiEnd();
  }
  GetInstanceWindow()->EndDrawing();
}

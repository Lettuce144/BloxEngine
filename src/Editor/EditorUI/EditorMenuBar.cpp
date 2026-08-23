#include <Core/engine.h>
#include <EditorUI/EditorMenuBar.h>
#include <editor.hpp>

#include <imgui.h>

void EditorMenuBar::DrawElement()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New project", "Ctrl+N"))
      {
        m_ShowNewProjectPopup = true;
      }

      if (ImGui::MenuItem("Open project", "Ctrl+O"))
      {
      }

      if (ImGui::MenuItem("Open Scene", "Ctrl+Shift+O"))
      {
        BloxEngine::Engine::OpenFile(
            [this](const std::string &path)
            { m_pEditor.OpenScene(path); },
            {{"Model Files", "toml,bloxscn"}});
      }

      if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
      {
        // editor->SaveScene(); TODO: Implement SaveScene
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
      if (ImGui::MenuItem("Undo", "Ctrl+Z"))
      {
        // editor->Undo(); TODO: Implement Undo
      }

      if (ImGui::MenuItem("Redo", "Ctrl+Y"))
      {
        // editor->Redo(); TODO: Implement Redo
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Entity"))
    {
      if (ImGui::MenuItem("Create empty Entity"))
      {
        // if(m_pEditor)
        m_pEditor.GetActiveScene()->CreateEntity("Entity");
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Debug", "F12"))
    {
      if (ImGui::MenuItem("Frame debug"))
      {
        // RendererDebug::DebugFrame(GameInstance *ptr)
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
      // editor->ShowAbout(); TODO: Implement ShowAbout
      if (ImGui::MenuItem("Show Demo"))
      {
        ImGui::ShowDemoWindow();
      }

      if (ImGui::MenuItem("About"))
      {
        if (ImGui::Begin("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
          ImGui::Text("Blox Engine Editor");
          ImGui::Text("Alpha 0.0.1");
          ImGui::Text("Developed by Lettuce144");
          ImGui::Text("2023 - present");
          ImGui::End();
        }
      }
      ImGui::EndMenu();
    }
  }

  ImGui::EndMainMenuBar();

  DrawPopUp();
}

void EditorMenuBar::DrawPopUp()
{

  if (m_ShowNewProjectPopup)
  {
    ImGui::OpenPopup("New Project");
    m_ShowNewProjectPopup = false;
  }

  ImGui::SetNextWindowSize(ImVec2(500, 0), ImGuiCond_Appearing);

  if (ImGui::BeginPopupModal(
          "New Project",
          nullptr,
          ImGuiWindowFlags_AlwaysAutoResize))

  {
    static char projectName[256] = "Untitled Project";
    static char projectPath[512] = "";

    ImGui::Text("Create a new project");
    ImGui::Separator();

    ImGui::Spacing();

    ImGui::Text("Project Name");
    ImGui::InputText(
        "##ProjectName",
        projectName,
        IM_ARRAYSIZE(projectName));

    ImGui::Spacing();

    ImGui::Text("Project Location");
    ImGui::InputText(
        "##ProjectPath",
        projectPath,
        IM_ARRAYSIZE(projectPath));

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Geometry Type");

    static int geometryType = 0;

    ImGui::RadioButton("2D", &geometryType, 0);
    ImGui::SameLine();

    ImGui::RadioButton("3D", &geometryType, 1);
    ImGui::SameLine();

    ImGui::RadioButton("2D + 3D", &geometryType, 2);

    ImGui::Spacing();

    // Optional description
    switch (geometryType)
    {
    case 0:
      ImGui::TextDisabled("Create a project focused on csg brush based geometry.");
      break;

    case 1:
      ImGui::TextDisabled("Create a project focused on mesh based geometry.");
      break;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Create", ImVec2(120, 0)))
    {
      // CreateProject(
      //     projectName,
      //     projectPath,
      //     geometryType
      // );

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

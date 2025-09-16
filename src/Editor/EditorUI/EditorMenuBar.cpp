#include <Core/engine.h>
#include <EditorUI/EditorMenuBar.h>
#include <editor.hpp>

#include <imgui.h>

void EditorMenuBar::DrawElement() const
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New project", "Ctrl+N"))
      {
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
        if(ImGui::Begin("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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
}

#include "gameinstance.h"
#include "baseentity.hpp"
#include "components/components.h"
#include "scene.h"

#include "EditorUI/EditorPanel.h"
#include "EditorUI/EditorButton.h"

#include "EditorUI/SceneHierarchyPanel.h"
#include "EditorUI/EditorViewportPanel.h"

// libraries are always included through the angle brackets
#include <imgui.h>
#include <rlImGui.h>
#include <raylib-cpp.hpp>

#include <iostream>

static void callbackTest()
{
  std::cout << "Pressed button!" << std::endl;
}

GameInstance::GameInstance(const int &width, const int &height, const char *title)
    : m_ptrActiveScene(std::make_shared<BloxEngine::Scene>())
{
  // TODO: Move this to the game project
  raylib::Window window(width, height, title, FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
  // raylib::Color textColor = raylib::Color::LightGray();

  raylib::Camera3D camera(raylib::Vector3{0.0f, 10.0f, 10.0f},
                          raylib::Vector3{0.0f, 0.0f, 0.0f},
                          raylib::Vector3{0.0f, 1.0f, 0.0f},
                          45.0f, CameraProjection::CAMERA_PERSPECTIVE);

  window.SetTargetFPS(60);

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

  DisableCursor(); // TODO: Fix this, it's not working

  // Editor panel
  BloxEngine::EditorUI::EditorPanel editorpanel;
  editorpanel.AddElement(std::make_unique<BloxEngine::EditorUI::EditorButton>("Hello, world!", ImVec2(100, 50), callbackTest));

  BloxEngine::EditorUI::SceneHierarchyPanel sceneHierarchyPanel(m_ptrActiveScene);

  // Editor viewport panel
  BloxEngine::EditorUI::EditorViewportPanel viewportpanel(camera, m_ptrActiveScene);

  // raylib::RenderTexture2D texture(window.GetWidth(), window.GetHeight());

  // --------------------------------------------------
  // Game specific code
  // --------------------------------------------------

  // Jeez only the standard Raylib api works without leaking memory, wtf?
  Model model = LoadModel("assets/radio.glb");
  raylib::Texture2D albedoModel("assets/wood.png");

  auto entity = m_ptrActiveScene->CreateEntity("Example radio model");
  entity.AddComponent<BloxEngine::ModelComponent>(model);

  auto block = m_ptrActiveScene->CreateEntity("Block");
  block.AddComponent<BloxEngine::ModelComponent>(LoadModel("assets/block.obj"));
  // Set the material for the block
  block.AddComponent<BloxEngine::ModelMaterialComponent>().SetMaterial(block, MATERIAL_MAP_DIFFUSE, albedoModel);

  // --------------------------------------------------
  // Raylib game loop
  // --------------------------------------------------

  while (!window.ShouldClose())
  {
    camera.Update(CAMERA_FREE);
    window.BeginDrawing();
    {
      window.ClearBackground(raylib::Color::FromHSV(34, 180, 30));

      rlImGuiBegin();
      ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

      sceneHierarchyPanel.Draw();
      editorpanel.Draw();
      viewportpanel.Draw();

      ImGui::ShowDemoWindow();

      viewportpanel.UpdateViewport();

      rlImGuiEnd();
    }
    window.EndDrawing();
  }
  // viewportpanel.~viewportpanel();
  rlImGuiShutdown();
}

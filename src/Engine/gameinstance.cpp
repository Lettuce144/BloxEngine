#include "gameinstance.h"
#include "baseentity.hpp"
#include "components/components.h"
#include "scene.h"

#include "raylib-cpp.hpp"

GameInstance::GameInstance(const int &width, const int &height, const char *title)
    : m_ptrActiveScene(std::make_shared<BloxEngine::Scene>())
{
  // TODO: Move this to the game project
  raylib::Window window(width, height, title);
  // raylib::Color textColor = raylib::Color::LightGray();

  raylib::Camera3D camera(raylib::Vector3{0.0f, 10.0f, 10.0f},
                          raylib::Vector3{0.0f, 0.0f, 0.0f},
                          raylib::Vector3{0.0f, 1.0f, 0.0f},
                          45.0f, CameraProjection::CAMERA_PERSPECTIVE);

  window.SetTargetFPS(60);
  // DisableCursor();

  // Jeez only the standard Raylib api works without leaking memory, wtf?
  Model model = LoadModel("assets/radio.glb");
  raylib::Texture2D albedoModel("assets/wood.png");

  auto entity = m_ptrActiveScene->CreateEntity("Example radio model");
  entity.AddComponent<BloxEngine::ModelComponent>(model);
  entity.AddComponent<BloxEngine::ModelMaterialComponent>().SetMaterial(entity, MATERIAL_MAP_DIFFUSE, albedoModel);

  auto block = m_ptrActiveScene->CreateEntity("Block");
  block.AddComponent<BloxEngine::ModelComponent>(LoadModel("assets/block.obj"));

  // Set the material for the block
  block.AddComponent<BloxEngine::ModelMaterialComponent>().SetMaterial(block, MATERIAL_MAP_DIFFUSE, albedoModel);

  while (!window.ShouldClose())
  {
    camera.Update(CAMERA_FREE);
    window.BeginDrawing();
    {
      window.ClearBackground(raylib::Color::FromHSV(34, 180, 30));
      // textColor.DrawText("Congrats! You created your first window!", 190, 200, 20);

      camera.BeginMode();
      {
        m_ptrActiveScene->OnUpdate();

        DrawPlane(raylib::Vector3{0.0f, 0.0f, 0.0f}, raylib::Vector2{32.0f, 32.0f}, GRAY);
      }
      camera.EndMode();
    }
    window.EndDrawing();
  }
}

#include "Application/gameinstance.h"
#include "baseentity.hpp"
#include "components/components.h"
#include "scene.h"

// libraries are always included through the angle brackets
#include <imgui.h>
#include <rlImGui.h>
#include <raylib-cpp.hpp>

#include <iostream>

namespace BloxEngine
{
  GameInstance::GameInstance(const InstanceProperties &props)
      : m_ptrActiveScene(std::make_shared<BloxEngine::Scene>())
  {

    m_ptrWindow = std::make_unique<raylib::Window>(props.width, props.height, props.appName, FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    m_ptrWindow->SetTargetFPS(60);

    SetExitKey(KEY_NULL);

    // --------------------------------------------------
    // Game specific code
    // --------------------------------------------------

    // Jeez only the standard Raylib api works without leaking memory, wtf?
    Model model = LoadModel("assets/radio.glb");

    auto entity = m_ptrActiveScene->CreateEntity("Example radio model");
    entity.AddComponent<BloxEngine::ModelComponent>(model);

    auto block = m_ptrActiveScene->CreateEntity("Block");
    block.AddComponent<BloxEngine::ModelComponent>(LoadModel("assets/block.obj"));

    // Set the material for the block
    raylib::Texture2D albedoModel("assets/wood.png");
    block.AddComponent<BloxEngine::ModelMaterialComponent>().SetMaterial(block, MATERIAL_MAP_DIFFUSE, albedoModel);
  }

  void GameInstance::Run()
  {
    OnAttach();

    while (!m_ptrWindow->ShouldClose())
    {
      OnUpdate();
    }

    OnDetach();
  }

} // namespace BloxEngine
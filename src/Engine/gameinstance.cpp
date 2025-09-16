#include "Application/gameinstance.h"
#include "baseentity.hpp"
#include "components/components.h"
#include "Renderer/forwardrenderer.h"
#include "scene.h"

// libraries are always included through the angle brackets
#include <raylib-cpp.hpp>

namespace BloxEngine
{
  GameInstance::GameInstance(const InstanceProperties &props)
  {

    m_ptrWindow = std::make_unique<raylib::Window>(
        props.width, props.height, props.appName,
        FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE |
            FLAG_WINDOW_HIGHDPI);
    // m_ptrWindow->SetTargetFPS(144);
    m_ptrActiveScene = std::make_shared<BloxEngine::Scene>();

    SetExitKey(KEY_NULL);

    // --------------------------------------------------
    // Engine specific code
    // --------------------------------------------------

    // TODO NOW: move these back to local scope
    m_ptrAssetsFS = std::make_unique<vfspp::NativeFileSystem>("assets/");
    m_ptrProjectsFS = std::make_unique<vfspp::NativeFileSystem>("projects/");
    m_ptrResourcesFS = std::make_unique<vfspp::NativeFileSystem>("resources/");

    m_ptrAssetsFS->Initialize();
    m_ptrProjectsFS->Initialize();
    m_ptrResourcesFS->Initialize();

    vfs = std::make_shared<vfspp::VirtualFileSystem>();

    vfs->AddFileSystem("/assets", std::move(m_ptrAssetsFS));
    vfs->AddFileSystem("/resources", std::move(m_ptrProjectsFS));

    // TODO: This should only be used on projects that compiled with the EDITOR_INSTANCE flag
    vfs->AddFileSystem("/projects", std::move(m_ptrResourcesFS));

    // --------------------------------------------------
    // Game specific code
    // --------------------------------------------------

    // Load models
    Model model = LoadModel("assets/radio.obj");
    model.materials[0].shader = ForwardRenderer::GetForwardLitShader();

    auto entity = m_ptrActiveScene->CreateEntity("Example radio model");
    entity.AddComponent<BloxEngine::ModelComponent>(model);

    Model blockModel = LoadModel("assets/block.obj");
    blockModel.materials[0].shader = ForwardRenderer::GetForwardLitShader();

    auto block = m_ptrActiveScene->CreateEntity("Block model");
    block.AddComponent<BloxEngine::ModelComponent>(blockModel);

    auto sunlight = m_ptrActiveScene->CreateEntity("Directional light");
    sunlight.AddComponent<BloxEngine::LightComponent>(
        Color{255, 251, 182, 255}, BloxEngine::LightComponent::LightType::Directional);
    sunlight.GetComponent<BloxEngine::TransformComponent>().translation = {0.0f, 4.0f, 0.0f};
    sunlight.GetComponent<BloxEngine::TransformComponent>().rotation = {45.0f, 45.0f, 90.0f};

    auto light = m_ptrActiveScene->CreateEntity("Simple point light");
    light.AddComponent<BloxEngine::LightComponent>(
        RED, BloxEngine::LightComponent::LightType::Point);
    light.GetComponent<BloxEngine::TransformComponent>().translation = {0.0f, 4.0f, 0.0f};

    // // Set the material for the block
    // raylib::Texture2D albedoModel = LoadTexture("assets/radio_albedo.png");
    // block.AddComponent<BloxEngine::ModelMaterialComponent>().SetMaterial(
    //     block, MATERIAL_MAP_DIFFUSE, albedoModel);
  }

  void GameInstance::Run()
  {
    while (!m_ptrWindow->ShouldClose())
    {
      OnUpdate(); // Will be called by a derived class i.e our Editor
    }
  }

  void GameInstance::OpenScene(const std::string &path)
  {
    // TODO: Check if the scene is already open
    // TODO: Implement OnSceneClose

    if (auto scenefile = vfs->OpenFile(vfspp::FileInfo(path), vfspp::IFile::FileMode::Read))
    {
      if (scenefile->IsOpened())
      {
        SceneSerializer serializer(m_ptrActiveScene);

        std::stringstream scenedata;
        scenefile->Read<std::stringstream>(scenedata);

        try
        {
          toml::table parsedToml = toml::parse(scenedata.str());
          serializer.Deserialize(parsedToml);
        }
        catch (const toml::parse_error &err)
        {
          std::cerr << "Parsing failed: " << err.what() << std::endl;
        }
      }
    }
  }

} // namespace BloxEngine

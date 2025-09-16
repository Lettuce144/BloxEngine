#include "scene.h"
#include "components/components.h"

#include "baseentity.hpp"
#include "Renderer/forwardrenderer.h"

#include <memory>
#include <iostream>

#define GIZMO_SPRITE_SIZE 1.0f

namespace BloxEngine
{

    Scene::Scene()
    {
        // TODO: Find out a way to abstract the assignment of gizmo icons to entities
        m_LightIcon = LoadTexture("resources/editor_sprites/light.png");
        m_SunIcon = LoadTexture("resources/editor_sprites/sun.png");

        ForwardRenderer::Init();
        ForwardRenderer::InitSkybox();
    }

    Scene::~Scene()
    {
        ForwardRenderer::Shutdown();
    }

    BaseEntity Scene::CreateEntity(const std::string &name)
    {
        // Every entity has a transform component and a tag component
        BaseEntity entity = BaseEntity(m_Registry.create(), shared_from_this());

        entity.AddComponent<BloxEngine::TransformComponent>();
        auto &tag = entity.AddComponent<BloxEngine::TagComponent>(name);

        tag.Tag = name.empty() ? "Entity" : name;

        return entity;
    }

    void Scene::DestroyEntity(BaseEntity entity)
    {
        // TODO: Make an entity map for loading and saving entities
        m_Registry.destroy(entity);
    }

    // TODO: This would be where the actual rendering of the scene would happen, for instance when actually playing the game
    // TODO: void Scene::RenderScene(EditorCamera& camera) for rendering the scene in the editor
    //       Basically seperate loop method, one for the editor and one for the game

    // Renders the editor world preview (no physics and such)
    void Scene::RenderEditorScene(Camera3D &currentCamera)
    {
        ForwardRenderer::RenderSkybox();

        ForwardRenderer::BeginFrame(currentCamera);

        for (entt::entity entity : m_Registry.view<TransformComponent, LightComponent>())
        {
            auto &transform = m_Registry.get<TransformComponent>(entity);
            auto &light = m_Registry.get<LightComponent>(entity);

            // Update the light shader with the current light's position and properties
            ForwardRenderer::SubmitLight(transform.translation, transform.rotation, light);

            if (light.type == LightComponent::LightType::Directional)
                ForwardRenderer::RenderGizmo(currentCamera, m_SunIcon, transform.translation, GIZMO_SPRITE_SIZE, WHITE);
            else
                ForwardRenderer::RenderGizmo(currentCamera, m_LightIcon, transform.translation, GIZMO_SPRITE_SIZE, WHITE);
        }

        ForwardRenderer::EndFrame();

        for (entt::entity entity : m_Registry.view<TransformComponent, ModelComponent>())
        {
            auto &transform = m_Registry.get<TransformComponent>(entity);
            auto &model = m_Registry.get<ModelComponent>(entity);

            Vector3 axis;
            float angle;
            transform.EulerToAxisAngle(transform.rotation, axis, angle);

            SetShaderValue(ForwardRenderer::GetForwardLitShader(),
                           ForwardRenderer::GetForwardLitShader().locs[SHADER_LOC_VECTOR_VIEW],
                           &currentCamera.position, SHADER_UNIFORM_VEC3);

            BeginShaderMode(ForwardRenderer::GetForwardLitShader());
            {
                model.ModelObject.Draw(transform.translation, axis, angle, transform.scale);
            }
            EndShaderMode();
        }

        // Called here so that it isn't affected by lights
        // ForwardRenderer::RenderDynamicSky(currentCamera);//(m_ptrGameInstance->GetActiveScene(), m_refCamera);
        DrawGrid(100, 1.0f); // Draw a grid with 100 lines, each 1.0f apart

        // TODO: Render terrain
        // for (entt::entity entity : m_Registry.view<TransformComponent, TerrainComponent>())
        // {
        //     auto &transform = m_Registry.get<TransformComponent>(entity).transform;
        //     auto &terrain = m_Registry.get<TerrainComponent>(entity);

        //     terrain.TerrainMesh.Draw(terrain.TerrainMaterial, MatrixAdd);
        // }
    }

    // ------------------- SceneSerializer -------------------
    SceneSerializer::SceneSerializer(const std::shared_ptr<Scene> &scene)
        : m_Scene(scene)
    {
    }

    SceneSerializer::~SceneSerializer()
    {
    }

    // Save the scene to a file
    void SceneSerializer::Serialize(const std::string &filepath)
    {
    }

    // Load the scene from a file
    void SceneSerializer::Deserialize(toml::table &parsedToml)
    {
        // printf("%s", parsedToml["Scene"].value_or("No scene data found").c_str());
        std::cout << parsedToml["scene"]["name"].value_or("No scene data found") << std::endl;
    }
}

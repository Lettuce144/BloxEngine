#include "scene.h"
#include "components/components.h"

#include "baseentity.hpp"

#include <memory>
#include <vector>

namespace BloxEngine
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    BaseEntity Scene::CreateEntity(const std::string &name)
    {
        // Every entity has a transform component and a tag component
        BaseEntity entity = BaseEntity(m_Registry.create(), shared_from_this()); // TODO: std::make_shared<Scene>(this) use this instead?

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
    // TODO: void Scene::RenderScene(EditorCamera& camera) for rendering the scene in the editor
    //       Basically seperate loop method, one for the editor and one for the game

    void Scene::OnUpdate()
    {
        // Render 3D objects that have a transform and model component
        for (entt::entity entity : m_Registry.view<TransformComponent, ModelComponent>())
        {
            auto &transform = m_Registry.get<TransformComponent>(entity).transform;
            auto &model = m_Registry.get<ModelComponent>(entity);

            // TODO: Rewrite TransformComponent
            // Yuck, this is ugly
            Vector3 &rotation = transform.translation;
            float angle;
            QuaternionToAxisAngle(transform.rotation, &rotation, &angle);

            model.ModelObject.Draw(transform.translation, rotation, angle, transform.scale);
        }
    }
}
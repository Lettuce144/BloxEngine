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

    void Scene::OnUpdate()
    {
        // Render 3D objects that have a transform and model component
        for (auto entity : m_Registry.view<TransformComponent, ModelComponent>())
        {
            auto &transform = m_Registry.get<TransformComponent>(entity).transform;
            auto &model = m_Registry.get<ModelComponent>(entity);

            // Yuck, this is ugly
            Vector3 &rotation = transform.translation;
            float angle;
            QuaternionToAxisAngle(transform.rotation, &rotation, &angle);
            
            model.ModelObject.Draw(transform.translation, rotation, angle, transform.scale);
            //model.ModelObject.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = model.AlbedoMap;
        }
    }
}
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <entt/entt.hpp>
#include "raylib-cpp.hpp"

/*
* All of the components that can be used in BloxEngine.
*
* Abstraction of Raylib features like meshes etc.
* Mostly just wrappers around Raylib features.

* TODO: Make a seperate components file for BloxEngine features.
* TODO: Use raylib instead of raylib-cpp !!!!!!!
*/

class BaseEntity;

#include "scene.h"

namespace BloxEngine
{

    // A position, rotation and scale component
    struct TransformComponent
    {
        Transform transform = {Vector3{0.0f, 0.0f, 0.0f}, Quaternion{0.0f, 0.0f, 0.0f, 1.0f}, Vector3{1.0f, 1.0f, 1.0f}};

        TransformComponent() = default;
        TransformComponent(const TransformComponent &) = default;
        TransformComponent(const Vector3 &translation, const Quaternion &rotation, const Vector3 &scale)
            : transform{translation, rotation, scale} {}

        // Implicit conversion to Transform (raylib)
        operator Transform() const { return transform; }

        // Delete the copy assignment operator
        TransformComponent &operator=(const TransformComponent &) = delete;
    };

    // Gives an entity a name, default and required!
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent &) = default;
        TagComponent(const std::string &tag) : Tag(tag) {}

        // Delete the copy assignment operator
        TagComponent &operator=(const TagComponent &) = delete;
    };

    // TODO: Probably gonna remove this later since using it is a bit redundant
    struct ModelMaterialComponent
    {
        raylib::Texture2D AlbedoMap;
        raylib::Texture2D NormalMap;
        raylib::Texture2D MetalnessMap;
        raylib::Texture2D RoughnessMap;
        raylib::Texture2D OcclusionMap;

        std::array<Texture2D, 5> MaterialMaps = {AlbedoMap, NormalMap, MetalnessMap, RoughnessMap, OcclusionMap};

        // @param entity The entity to set the material for
        // @param materialMapIndex Type of material
        // @param texture The texture to set
        void SetMaterial(BaseEntity& entity, const MaterialMapIndex &materialMapIndex, const Texture2D &texture);

        ModelMaterialComponent() = default;
        ModelMaterialComponent(const ModelMaterialComponent &) = default;
        ModelMaterialComponent(const Texture2D &albedoMap, const Texture2D &normalMap, const Texture2D &metalnessMap, const Texture2D &roughnessMap, const Texture2D &occlusionMap)
            : AlbedoMap(albedoMap), NormalMap(normalMap), MetalnessMap(metalnessMap), RoughnessMap(roughnessMap), OcclusionMap(occlusionMap)
        {
        }

        // Delete the copy assignment operator
        ModelMaterialComponent &operator=(const ModelMaterialComponent &) = delete;
    };

    struct ModelComponent
    {
        // BUG: Using raylib::Model in the constructer causes a segfault when the destructor is called
        // SOLUTION: Use Model instead of the cpp wrapper raylib::Model, alternatively use a shared_ptr here
        // TODO: Use a shared_ptr?
        raylib::Model ModelObject;
        // raylib::Texture2D AlbedoMap;

        ModelComponent() = default;
        ModelComponent(const ModelComponent &) = default;
        ModelComponent(const Model &model) : ModelObject(model) {}

        // Implicit conversion to Model (raylib)
        operator Model() const { return ModelObject; }

        ModelComponent &operator=(const ModelComponent &) = delete;
    };

    // NOTE: This is a wrapper around the raylib Model class
    // TODO: Change this to a Model
    // struct MeshComponent
    // {
    //     raylib::Mesh MeshObject;
    //     Material MeshMaterial;

    //     MeshComponent() = default;
    //     MeshComponent(const MeshComponent &) = default;
    //     MeshComponent(const Mesh &mesh, const Material& material) : MeshObject(mesh), MeshMaterial(material) {}

    //     // Implicit conversion to Transform (raylib)
    //     operator Mesh() const { return MeshObject; }

    //     // Delete the copy assignment operator
    //     MeshComponent& operator=(const MeshComponent&) = delete;
    // };
}

#endif // COMPONENTS_H
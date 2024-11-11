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
*/

namespace BloxEngine
{

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

    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent &) = default;
        TagComponent(const std::string &tag) : Tag(tag) {}

        // Delete the copy assignment operator
        TagComponent &operator=(const TagComponent &) = delete;
    };

    struct ModelComponent
    {
        raylib::Model ModelObject;

        // Todo make this a material component
        raylib::Texture2D AlbedoMap;
        /*
        raylib::Texture2D NormalMap;
        raylib::Texture2D SpecularMap;
        raylib::Texture2D EmissionMap;
        */

        ModelComponent() = default;
        ModelComponent(const ModelComponent &) = default;
        ModelComponent(const Model &model) : ModelObject(model) {}
        ModelComponent(const Model &model, const Texture2D &material) : ModelObject(model), AlbedoMap(material) {}
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
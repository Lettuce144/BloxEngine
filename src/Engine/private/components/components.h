#ifndef COMPONENTS_H
#define COMPONENTS_H

// #include <entt/entt.hpp>
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
        Vector3 translation = {0.0f, 0.0f, 0.0f};
        Vector3 rotation = {0.0f, 0.0f, 0.0f}; // Euler angles in degrees
        Vector3 scale = {1.0f, 1.0f, 1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent &) = default;
        TransformComponent(const Vector3 &translation, const Vector3 &rotation, const Vector3 &scale)
            : translation(translation), rotation(rotation), scale(scale) {}

        // TODO: Move this to a utility file or class
        //  Utility function to convert Euler (in degrees) to axis-angle
        void EulerToAxisAngle(const Vector3 &eulerDegrees, Vector3 &outAxis, float &outAngle)
        {
            // Convert degrees to radians
            Vector3 euler = Vector3Scale(eulerDegrees, DEG2RAD);

            // Create quaternion from euler
            Quaternion q = QuaternionFromEuler(euler.x, euler.y, euler.z);

            // Normalize quaternion just in case
            q = QuaternionNormalize(q);

            // Convert to axis-angle
            outAngle = 2.0f * acosf(q.w);

            float sinHalfAngle = sqrtf(1.0f - q.w * q.w);
            if (sinHalfAngle < 0.001f)
            {
                // If the angle is too small, set arbitrary axis
                outAxis = {1.0f, 0.0f, 0.0f};
            }
            else
            {
                outAxis.x = q.x / sinHalfAngle;
                outAxis.y = q.y / sinHalfAngle;
                outAxis.z = q.z / sinHalfAngle;
            }

            // Convert angle from radians to degrees if needed by your renderer
            outAngle *= RAD2DEG; // Only if your Draw expects degrees (Raylib uses radians, so skip this line if not needed)
        }

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
        void SetMaterial(BaseEntity &entity, const MaterialMapIndex &materialMapIndex, const Texture2D &texture);

        ModelMaterialComponent() = default;
        ModelMaterialComponent(const ModelMaterialComponent &) = delete;
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
        ModelComponent(const ModelComponent &) = delete;
        ModelComponent(const Model &model) : ModelObject(model) {}

        void setModel(const std::string &modelPath)
        {
            ModelObject.Unload();

            // ModelObject.Load(modelPath.c_str()); // TODO: Catch errors
            try
            {
                ModelObject.Load(modelPath.c_str());
                // ModelObject = LoadModel(modelPath.c_str());
            }
            catch (const std::exception &e)
            {
                printf("Error loading model: %s\n", e.what());
            }
        }

        // @warning This overload doesn't unload the previous model!
        void setModel(const Model &model)
        {
            ModelObject = model;
        }

        // Implicit conversion to Model (raylib)
        operator Model() const { return ModelObject; }

        ModelComponent &operator=(const ModelComponent &) = delete;
    };

    struct TerrainComponent
    {
        raylib::Mesh TerrainMesh;
        raylib::Material TerrainMaterial;

        // Look into this!
        // GenImageWhiteNoise
        // GenImagePerlinNoise

        TerrainComponent() = default;
        TerrainComponent(const TerrainComponent &) = delete;
        TerrainComponent(const Mesh &mesh, const Material &texture) : TerrainMesh(mesh), TerrainMaterial(texture)
        {
            TerrainMesh = GenMeshPlane(1.0f, 1.0f, 10, 10);
        }

        // Implicit conversion to Mesh (raylib)
        operator Mesh() const { return TerrainMesh; }

        // Delete the copy assignment operator
        TerrainComponent &operator=(const TerrainComponent &) = delete;
    };

    // Point light component
    struct LightComponent
    {

        Color color = WHITE;
        enum class LightType
        {
            Point,
            Directional,

        } type = LightType::Point;
        Vector3 target = {0, 0, 0};

        // Unused
        // bool enabled;
        // bool enableShadow;
        // float shadowBias;
        // float shadowIntensity;

        LightComponent() = default;
        LightComponent(const LightComponent &) = default;
        LightComponent(const Color &col, LightType lightType = LightType::Point)
            : type(lightType), color(col) {}

        LightComponent &operator=(const LightComponent &) = default;
    };
}

#endif // COMPONENTS_H

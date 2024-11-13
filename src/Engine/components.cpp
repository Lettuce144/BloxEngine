#include "components/components.h"
#include "baseentity.hpp"

namespace BloxEngine{
    void ModelMaterialComponent::SetMaterial(BaseEntity& entity, const MaterialMapIndex &materialMapIndex, const Texture2D &texture)
    {
        auto &model = entity.GetComponent<ModelComponent>().ModelObject;
        model.materials[0].maps[materialMapIndex].texture = texture;
    }
}
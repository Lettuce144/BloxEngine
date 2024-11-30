#include "baseentity.hpp"
#include "components/components.h"

namespace BloxEngine
{
    BaseEntity::BaseEntity(entt::entity handle, std::shared_ptr<Scene> scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

    const std::string& BaseEntity::GetName() const
    {
        return GetComponent<TagComponent>().Tag;
    }
}
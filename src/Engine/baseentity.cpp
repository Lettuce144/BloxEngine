#include "baseentity.hpp"

namespace BloxEngine
{
    BaseEntity::BaseEntity(entt::entity handle, std::shared_ptr<Scene> scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }
}
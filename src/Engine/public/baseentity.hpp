#ifndef BASEENTITY_HPP
#define BASEENTITY_HPP
#pragma once

#include "scene.h"
#include "components/components.h"

#include <memory>

#include "raylib-cpp.hpp"
#include <entt/entt.hpp>

// Huge credit https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Scene/Entity.h

namespace BloxEngine
{
    // Base entity class, wrapper for getting the entity from the registry.
    // All of the entity's components are stored in the registry which is stored in scene
    // Now talking to the scene directly for interacting with entities from a user perspective doensn't make sense
    // So we have this class to handle that
    class BaseEntity
    {
    public:
        BaseEntity(const BaseEntity &other) = default;
        BaseEntity(entt::entity handle, std::shared_ptr<Scene> scene);

        template <typename T, typename... Args>
        T &AddComponent(Args &&...args)
        {
            // Add a component to the registry
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template <typename T>
        T &GetComponent() const
        {
            // Get a component from the registry
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template <typename T>
        bool HasComponent()
        {
            // Get a component from the registry
            return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
        }

        template <typename T>
        void RemoveComponent()
        {
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        const std::string &GetName() const { return GetComponent<TagComponent>().Tag; }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }

    private:
        entt::entity m_EntityHandle{entt::null};
        std::shared_ptr<Scene> m_Scene; // I'm not making my own ptr wrapper class blegh
    };
}
#endif // BASEENTITY_HPP
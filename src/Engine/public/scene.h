#ifndef SCENE_H
#define SCENE_H
#pragma once

#include <entt/entt.hpp>

namespace BloxEngine
{
	// Forward declaration of BaseEntity
	class BaseEntity;

	// Example taken from https://www.youtube.com/watch?v=D4hz0wEB978
	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		Scene();
		~Scene();

		void OnUpdate();

		BaseEntity CreateEntity(const std::string &name);
		void DestroyEntity(BaseEntity entity);

	private:
		entt::registry m_Registry;
		friend class BaseEntity;
	};
#endif /* SCENE_H */
}
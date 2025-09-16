#ifndef SCENE_H
#define SCENE_H
#pragma once

#include <entt/entt.hpp>
#include <toml++/toml.hpp>
#include <raylib.h>

namespace BloxEngine
{
	// Forward declaration of BaseEntity
	class BaseEntity;

	namespace EditorUI
	{
		class SceneHierarchyPanel; // Forward declaration of SceneHierarchyPanel
	}

	// Example taken from https://www.youtube.com/watch?v=D4hz0wEB978
	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		Scene();
		~Scene();

		void RenderEditorScene(Camera3D &currentCamera);

		BaseEntity CreateEntity(const std::string &name);
		void DestroyEntity(BaseEntity entity);

	private:
		entt::registry m_Registry;
		Texture2D m_LightIcon;
		Texture2D m_SunIcon;

		// TODO: When adding lua scripting, add a lua state here

		friend class BaseEntity;
		friend class EditorUI::SceneHierarchyPanel;
	};

	class SceneSerializer
	{
	public:
		SceneSerializer(const std::shared_ptr<Scene> &scene);
		~SceneSerializer();

		void Serialize(const std::string &filepath);
		void Deserialize(toml::table &parsedToml);

	private:
		std::shared_ptr<Scene> m_Scene;
	};
#endif /* SCENE_H */
};

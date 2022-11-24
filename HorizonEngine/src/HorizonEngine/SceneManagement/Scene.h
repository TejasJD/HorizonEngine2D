#pragma once

#ifndef HZN_SCENE_H
#define HZN_SCENE_H

#include <entt/entt.hpp>
#include <cereal/archives/json.hpp>
#include <glm/glm.hpp>

#include <algorithm>

#include "HorizonEngine/Core/TimeStep.h"
#include "HorizonEngine/Camera/CameraController.h"

namespace Hzn
{
	class SceneManager;
	class ProjectManager;

	class Scene
	{
		friend class GameObject;
		friend class SceneManager;
		friend class ProjectManager;
		friend class AssetManager;
	public:
		Scene();
		Scene(cereal::JSONInputArchive& inputArchive);
		~Scene();

		glm::vec2 onViewportResize(uint32_t width, uint32_t height);
		void Scene::onEditorUpdate(OrthographicCamera& camera, TimeStep ts);
		void onUpdate(TimeStep ts);
		/**
		 * \brief creates a game object in the scene and returns a valid game object.
		 * \return Valid Game Object.
		 */
		GameObject createGameObject(const std::string& name);
		/**
		 * \brief Deletes the game object from the scene, and invalidates the variable that
		 * represented this Game object.
		 * \param obj Reference to the Game Object.
		 */
		void destroyGameObject(GameObject& obj);

		GameObject getGameObject(const std::string& name) const;
		GameObject getGameObject(uint32_t id) const;

		std::vector<std::string> allGameObjectNames() const;
		std::vector<std::string> getAllRootObjects() const;

		std::vector<uint32_t> getAllRootIds() const;
		std::vector<uint32_t> getAllObjectIds() const;

		std::filesystem::path getFilePath() const { return m_Path; }

	private:
		int gameObjectCounter = 0;
		void serialize(cereal::JSONOutputArchive& outputArchive);
		void invalidate();
		// variable that is used by the scene manager to invalidate all
		// the pointers to the scene. If the scene manager sets the scene.
		// then those pointers cannot perform actions on the scene.
		bool m_Valid = false;
		// entt registry for creating game objects.
		entt::registry m_Registry;

		// unordered map for retrieving objects by name.
		std::unordered_map<std::string, GameObject> m_Objects;
		std::unordered_map<uint32_t, GameObject> m_LocStorage;
		// viewport size of the scene. Helps in maintaining the aspect ratio of the scene.
		glm::vec2 m_lastViewportSize = { 0.0f, 0.0f };
		std::filesystem::path m_Path = std::filesystem::path();
	};

}

#endif // !HZN_SCENE_H
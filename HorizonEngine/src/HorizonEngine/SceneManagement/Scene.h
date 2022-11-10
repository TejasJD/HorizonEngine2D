#pragma once

#ifndef HZN_SCENE_H
#define HZN_SCENE_H

#include <entt/entt.hpp>
#include <cereal/archives/json.hpp>
#include <glm/glm.hpp>

#include "HorizonEngine/Core/TimeStep.h"

namespace Hzn
{
	class SceneManager;

	class Scene
	{
		friend class GameObject;
		friend class SceneManager;
	public:
		Scene();
		Scene(cereal::JSONInputArchive& inputArchive);
		~Scene();

		glm::vec2 onViewportResize(uint32_t width, uint32_t height);
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

		GameObject getGameObject(const std::string& name);

		std::vector<std::string> allGameObjectNames() const;
		std::vector<std::string> getAllRootObjects() const;

	private:
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
		// viewport size of the scene. Helps in maintaining the aspect ratio of the scene.
		glm::vec2 m_lastViewportSize = { 0.0f, 0.0f };
	};

}

#endif // !HZN_SCENE_H

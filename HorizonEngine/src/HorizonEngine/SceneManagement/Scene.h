#pragma once

#ifndef HZN_SCENE_H
#define HZN_SCENE_H

#include <entt/entt.hpp>
#include <cereal/archives/json.hpp>
#include <glm/glm.hpp>

#include <algorithm>

#include "HorizonEngine/Core/TimeStep.h"
#include "HorizonEngine/Camera/CameraController.h"

class b2World;

namespace Hzn
{
	class SceneManager;
	class ProjectManager;

	enum class SceneState
	{
		Edit = 0,
		Play = 1
	};

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

		glm::vec2 onViewportResize(int32_t width, int32_t height);

		void onStart();
		void onStop();

		GameObject getActiveCamera();

		void onEditorUpdate(OrthographicCamera& camera, TimeStep ts);
		void onUpdate(TimeStep ts);

		GameObject createGameObject(const std::string& name);
		void destroyGameObject(GameObject& obj);
		GameObject getGameObjectById(uint32_t id);

		std::vector<uint32_t> getAllRootIds() const;
		std::vector<uint32_t> getAllObjectIds() const;

		std::filesystem::path getFilePath() const { return m_Path; }
		std::string getName() const { return getFilePath().filename().string(); }


	private:
		void serialize(cereal::JSONOutputArchive& outputArchive);
		void invalidate();
		// variable that is used by the scene manager to invalidate all
		// the pointers to the scene. If the scene manager sets the scene.
		// then those pointers cannot perform actions on the scene.
		bool m_Valid = false;
		// entt registry for creating game objects.
		entt::registry m_Registry;

		b2World* m_World = nullptr;

		std::unordered_map<uint32_t, entt::entity> m_GameObjectIdMap;
		// viewport size of the scene. Helps in maintaining the aspect ratio of the scene.
		glm::vec2 m_lastViewportSize = { 0.0f, 0.0f };
		std::filesystem::path m_Path = std::filesystem::path();

		std::vector<uint32_t> m_ObjectsToDelete;

		SceneState m_State = SceneState::Edit;
	};

}

#endif // !HZN_SCENE_H
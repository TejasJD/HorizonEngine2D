#pragma once

#ifndef HZN_SCENE_MANAGER_H
#define HZN_SCENE_MANAGER_H

#include <glm/glm.hpp>
#include <HorizonEngine/Core/TimeStep.h>

namespace Hzn
{
	class OrthographicCamera;
	class Scene;
	enum class SceneState;

	class SceneManager
	{
	public:
		static std::shared_ptr<Scene> create(const std::filesystem::path& filepath);
		static std::shared_ptr<Scene> open(const std::filesystem::path& filepath);
		static glm::vec2 resizeViewport(int32_t width, int32_t height);

		static bool isOpen() { return s_Scene != nullptr; }
		static SceneState getSceneState();
		static void setSceneState(const SceneState& state);
		static std::shared_ptr<Scene> getActiveScene();

		static void play();
		static void update(OrthographicCamera& camera, TimeStep ts);
		static void stop();

		static bool close();
		static bool save();

	private:
		static void defaultScene();
		static std::shared_ptr<Scene> s_Scene;
	};
}

#endif
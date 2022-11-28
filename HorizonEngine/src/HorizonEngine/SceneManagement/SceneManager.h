#pragma once

#ifndef HZN_SCENE_MANAGER_H
#define HZN_SCENE_MANAGER_H

namespace Hzn
{
	class Scene;

	class SceneManager
	{
	public:
		static std::shared_ptr<Scene> create(const std::filesystem::path& filepath);
		static std::shared_ptr<Scene> open(const std::filesystem::path& filepath);
		static void update(TimeStep ts);
		static void play();
		static void stop();
		static void close();
		static void save();
	private:
		static void defaultScene();
		static std::shared_ptr<Scene> s_Scene;
	};
}

#endif
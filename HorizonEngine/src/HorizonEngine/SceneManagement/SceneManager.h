#pragma once

#ifndef HZN_SCENE_MANAGER_H
#define HZN_SCENE_MANAGER_H

namespace Hzn
{
	class Scene;

	class SceneManager
	{
	public:
		static std::shared_ptr<Scene> load(const std::string& filepath = std::string());
		static void close(const std::string& filepath);

		static void save(const std::string& filepath);
	private:
		static std::string s_FilePath;
		static std::shared_ptr<Scene> s_Scene;
	};
}

#endif
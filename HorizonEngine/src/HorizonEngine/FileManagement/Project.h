#pragma once

#ifndef HZN_PROJECT_H
#define HZN_PROJECT_H

#include "HorizonEngine/SceneManagement/Scene.h"

namespace Hzn
{
	class Project
	{
		friend class ProjectManager;
		friend class SceneManager;
		friend class Scene;

	public:
		Project() = default;
		Project(const std::string& name, const std::filesystem::path& directoryPath);
		Project(const std::filesystem::path& projectFilePath);
		~Project() = default;

		bool setActiveScene(const std::filesystem::path& filepath);
		
		std::shared_ptr<Scene> getActiveScene() const { return m_Scene; }
		std::filesystem::path getPath() const { return m_Path; }

	private:
		std::filesystem::path m_Path;
		std::shared_ptr<Scene> m_Scene;
	};
}

#endif
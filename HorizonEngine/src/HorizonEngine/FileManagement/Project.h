#pragma once

#ifndef HZN_PROJECT_H
#define HZN_PROJECT_H

#include "HorizonEngine/SceneManagement/Scene.h"

namespace Hzn
{
	class Project
	{
		friend class ProjectManager;

	public:
		Project() = default;
		Project(const std::string& name, const std::filesystem::path& path);
		~Project() = default;

	private:
		std::filesystem::path m_Path;
		std::shared_ptr<Scene> m_Scene;
	};
}

#endif
#include "pch.h"
#include "ProjectManager.h"

namespace Hzn
{
	std::shared_ptr<Project> ProjectManager::s_Project = nullptr;

	std::shared_ptr<Project> ProjectManager::create(const std::string& name, const std::filesystem::path& directoryPath)
	{
		if(s_Project)
		{
			save();
		}

		return s_Project = std::make_shared<Project>(name, directoryPath);
	}

	std::shared_ptr<Project> ProjectManager::open(const std::filesystem::path& projectFilePath)
	{
	}

	void ProjectManager::save()
	{
		
	}

	void ProjectManager::close()
	{
		
	}
}
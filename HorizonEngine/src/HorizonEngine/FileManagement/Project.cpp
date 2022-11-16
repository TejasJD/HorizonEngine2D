#include "pch.h"
#include "Project.h"
namespace fs = std::filesystem;

namespace Hzn
{
	Project::Project(const std::string& name, const fs::path& directoryPath)
		: m_Path(directoryPath)
	{
		// then this is the a new project.
		if(fs::is_directory(directoryPath))
		{
			// generate the project directory and required subdirectories.
			fs::create_directory(name);
			// call scene manager to load a default scene.
		}
		else
		{
			// load project.
		}
	}

}
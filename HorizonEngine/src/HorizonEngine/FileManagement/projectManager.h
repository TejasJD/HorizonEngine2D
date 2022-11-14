#pragma once

#ifndef HZN_PROJECT_MANAGER_H
#define HZN_PROJECT_MANAGER_H

#include "SceneManagement/Scene.h"

namespace Hzn{
	//class ProjectFile;

	class ProjectManager{
	private:
		std::string projectPath;

	public:

		//Default constructor
		ProjectManager() {}
		//Constructor to set projectPath
		ProjectManager(std::string dirPath) { projectPath = dirPath; }

		//declare function to make project structure
		//will return scene pointer
		Scene mkProjDirs();
		
	};
}

#endif
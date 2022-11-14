#include "pch.h"
#include "projectManager.h"
#include "ProjectFile.h"
#include "SceneManagement/Scene.h"


namespace Hzn{
		//Function to make project structure
		Scene ProjectManager::mkProjDirs() {
			
			//Declare project obj
			Hzn::ProjectFile* pjf = new Hzn::ProjectFile();

			//Make root directory
			pjf->makeDir(projectPath + "/Horizon_project");
			//Make assets directory
			pjf->makeDir(projectPath + "/Horizon_project/assets");
			//make scenes directory
			pjf->makeDir(projectPath + "/Horizon_project/scenes");
			//make a blank file
			pjf ->mkNewfile(projectPath + "/Horizon_project/scenes/newScene.txt");

			//create a scene object for the new project
			Hzn::Scene* s = new Hzn::Scene(pjf);
			s->open();

			//return a scene pointer
			return *s;
		}
}
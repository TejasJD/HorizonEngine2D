#pragma once

#ifndef HZN_PROJECT_MANAGER_H
#define HZN_PROJECT_MANAGER_H
#include "Project.h"

namespace Hzn
{
	class Project;
	class SceneManager;

	class ProjectManager
	{
		friend class SceneManager;
	public:
		/**
		 * \brief Creates new, or opens a project depending on the path provided.
		 * If you provide a directory path, it generates a new project in that directory.
		 * If you provide a file path, it checks whether this path is to the file .hzn,
		 * and depending on that file, the project is loaded.
		 * \param name name of the folder to create project.
		 * \param directoryPath directory in which the project directory should be created.
		 * \return 
		 */
		static std::shared_ptr<Project> create(const std::string& name, const std::filesystem::path& directoryPath);
		static std::shared_ptr<Project> open(const std::filesystem::path& projectFilePath);

		static void newScene(const std::string& name);
		static void openScene(const std::string& name);
		static void save();
		static void close();

	private:
		static std::shared_ptr<Project> s_Project;
	};
}

#endif
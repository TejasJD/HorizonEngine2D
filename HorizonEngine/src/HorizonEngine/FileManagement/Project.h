#pragma once

#ifndef HZN_PROJECT_H
#define HZN_PROJECT_H

#include "HorizonEngine/SceneManagement/Scene.h"

namespace Hzn
{
	class Project
	{
		//! Gives Project Manager access to Project.h.
		friend class ProjectManager;
		//! Gives Scene Manager access to Project.h.
		friend class SceneManager;
		//! Gives Scene access to Project.h.
		friend class Scene;

	public:
		/// <summary>
		/// constructors
		/// </summary>
		Project() = default;
		Project(const std::string& name, const std::filesystem::path& directoryPath);
		Project(const std::filesystem::path& projectFilePath);
		~Project() = default;

		//! set the active scene
		bool setActiveScene(const std::filesystem::path& filepath);
		
		//! Get active Scene
		std::shared_ptr<Scene> getActiveScene() const { return m_Scene; }
		//! Get path
		std::filesystem::path getPath() const { return m_Path; }

		std::string getName() const;

	private:
		//!path variable
		std::filesystem::path m_Path;
		//!scene variable
		std::shared_ptr<Scene> m_Scene;
	};
}

#endif
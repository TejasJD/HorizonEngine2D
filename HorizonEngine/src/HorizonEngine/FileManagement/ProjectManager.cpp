#include "pch.h"
#include "ProjectManager.h"

#include <FileWatch.hpp>

#include "HorizonEngine/SceneManagement/SceneManager.h"
#include "AssetManagement/AssetManager.h"

namespace Hzn
{
	static std::unique_ptr<filewatch::FileWatch<std::wstring>> watch;
	std::shared_ptr<Project> ProjectManager::s_Project = nullptr;

	std::shared_ptr<Project> ProjectManager::create(const std::string& name, const std::filesystem::path& directoryPath)
	{
		if (s_Project)
		{
			close();
		}
		s_Project = std::make_shared<Project>(name, directoryPath);
		/*watch.reset(new filewatch::FileWatch<std::wstring>(s_Project->m_Path.parent_path().wstring(), [&]
		(const std::wstring& path, const filewatch::Event& fileEvent)
			{
				switch (fileEvent)
				{
				case filewatch::Event::added: {
					break;
				}
				case filewatch::Event::removed: {
					break;
				}
				case filewatch::Event::modified: {
					break;
				}
				case filewatch::Event::renamed_old: {
					break;
				}
				case filewatch::Event::renamed_new: {
					break;
				}
				default: {
					break;
				}
				}
			}));*/
		if (std::filesystem::exists(s_Project->m_Path.parent_path().string() + "\\bin\\ScriptAppLib.dll"))
		{
			ScriptEngine::ReloadAssembly();
		}
		return s_Project;
	}

	bool ProjectManager::newScene(const std::string& name)
	{
		if (!s_Project)
		{
			HZN_CORE_ASSERT(false, "trying to create new Scene in empty project!");
			return false;
		}
		std::filesystem::path projectDir = s_Project->m_Path.parent_path();
		std::filesystem::path sceneDir = projectDir.string() + "\\scenes";

		// create a scenes directory if it doesn't exist.
		if (!std::filesystem::exists(sceneDir))
		{
			std::filesystem::create_directory(sceneDir);
		}

		// create a scene file.
		std::filesystem::path sceneFilePath = sceneDir.string() + "\\" + name + ".scene";
		s_Project->m_Scene = SceneManager::create(sceneFilePath);

		return s_Project->m_Scene != nullptr;
	}

	bool ProjectManager::openScene(const std::filesystem::path& sceneFilePath)
	{
		if (!s_Project)
		{
			HZN_CORE_ASSERT(false, "trying to open Scene in an empty project!");
			return false;
		}
		std::filesystem::path projectDir = s_Project->m_Path.parent_path();
		std::filesystem::path sceneDir = projectDir.string() + "\\scenes";

		HZN_CORE_ASSERT(std::filesystem::exists(sceneDir), "no scenes directory!");

		s_Project->m_Scene = SceneManager::open(sceneFilePath);
		return s_Project->m_Scene != nullptr;
	}

	bool ProjectManager::closeScene()
	{
		if (s_Project && s_Project->m_Scene)
		{
			return SceneManager::close();
		}
		return false;
	}

	bool ProjectManager::saveScene()
	{
		if (s_Project && s_Project->m_Scene)
		{
			return SceneManager::save();
		}
		return false;
	}

	std::shared_ptr<Project> ProjectManager::open(const std::filesystem::path& projectFilePath)
	{
		if (!projectFilePath.empty()) {
			if (s_Project)
			{
				//AssetManager::destroy();
				close();
			}
			s_Project = std::make_shared<Project>(projectFilePath);

			// here we will be starting the file watcher on this location if the path exists.
			if (std::filesystem::exists(s_Project->m_Path.parent_path().string() + "\\bin\\ScriptAppLib.dll"))
			{
				ScriptEngine::ReloadAssembly();
				// we start it after reloading.
			}
		}

		return s_Project;
	}

	bool ProjectManager::save()
	{
		bool result = true;
		if (s_Project && s_Project->m_Scene)
		{
			std::filesystem::path projectFile = s_Project->m_Path;
			std::ofstream os(projectFile, std::ios::binary);
			os << "ActiveScene" << " " << ":" << " " << s_Project->m_Scene->m_Path.string();
			os.close();
			result = SceneManager::save();
		}
		return result;
	}

	bool ProjectManager::close()
	{
		bool result = save();
		if (s_Project && s_Project->m_Scene) {
			result = SceneManager::close();
			AssetManager::destroy();
			// here we should close the previous file watcher if it exists.
			s_Project.reset();
		}
		return result;
	}
}
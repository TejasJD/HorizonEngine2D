#include "pch.h"
#include "Project.h"
#include "HorizonEngine/SceneManagement/SceneManager.h"

namespace fs = std::filesystem;

namespace Hzn
{
	Project::Project(const std::string& name, const fs::path& directoryPath)
	{
		// then this is the a new project.
		HZN_CORE_ASSERT(fs::is_directory(directoryPath), "Provided path isn't a directory!");
		
		m_Path = fs::path(directoryPath.string() + "\\" + name);

		// generate the project directory.
		HZN_CORE_ASSERT(fs::create_directory(m_Path), "Directory couldn't be created!");

		m_Path = m_Path.string() + "\\" + name + ".hzn";

		// generate subdirectories
		fs::create_directory(m_Path.parent_path().string() + "\\textures");
		fs::create_directory(m_Path.parent_path().string() + "\\audios");
		fs::create_directory(m_Path.parent_path().string() + "\\scenes");
		fs::copy(fs::current_path().string() + "\\icons", m_Path.parent_path().string() + "\\icons");

		// create file.
		std::ofstream os(m_Path);
		os.close();
		
	}

	Project::Project(const std::filesystem::path& projectFilePath)
		: m_Path(projectFilePath)
	{
		HZN_CORE_ASSERT(fs::is_regular_file(m_Path), "Provided path is not a file");
		HZN_CORE_ASSERT(m_Path.extension() == ".hzn", "Provided file is not a project file!");

		std::ifstream is(projectFilePath);

		HZN_CORE_ASSERT(is, "failed to open project file!");

		std::string key, sep, value;
		is >> key >> sep >> value;
		if (!value.empty() && std::filesystem::exists(value) && std::filesystem::is_regular_file(value)) {
			m_Scene = SceneManager::open(value);
		}

		is.close();
	}

	bool Project::setActiveScene(const std::filesystem::path& filepath)
	{
		
		if (m_Scene = SceneManager::open(filepath))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

#include "pch.h"
#include "Project.h"
#include "HorizonEngine/SceneManagement/SceneManager.h"
#include "HorizonEngine/App.h"

namespace fs = std::filesystem;

namespace Hzn
{
	/// <summary>
	/// Initialisation of the Project constructor, takes in name and path
	/// Creates a project directory structure in the directory provided
	/// </summary>
	/// <param name="name"></param>
	/// <param name="directoryPath"></param>
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
		fs::create_directory(m_Path.parent_path().string() + "\\sprites");
		fs::create_directory(m_Path.parent_path().string() + "\\audios");
		fs::create_directory(m_Path.parent_path().string() + "\\scenes");
		fs::copy(fs::current_path().string() + "\\icons", m_Path.parent_path().string() + "\\icons");

		// set up script directories and files.
		fs::create_directories(m_Path.parent_path().string() + "\\ScriptAppLib\\Source");

		// directory for binaries (the dll is watched by the filewatcher in this library).
		fs::create_directory(m_Path.parent_path().string() + "\\bin");
		// directory from which we load dlls (they are copied from bin to load upon rebuild).
		fs::create_directory(m_Path.parent_path().string() + "\\load_target");

		// sample file for now.
		std::ofstream os(m_Path.parent_path().string() + "\\ScriptAppLib\\Source\\Player.cs");
		os.close();

		// copy lua file from executable to the project directory.
		fs::copy(fs::current_path().string() + "\\premake5.lua", m_Path.parent_path().string() + "\\premake5.lua");

		// execute premake with the project's lua file as --file argument.
		std::string premake = Hzn::App::getApp().getExecutablePath().string() + "\\premake5 --file=";
		std::string command = premake + m_Path.parent_path().string() + "\\premake5.lua" + " vs2022";
		system(command.c_str());

		// create file.
		os.open(m_Path);
		os.close();
	}

	/// <summary>
	/// Sets up scene file in the project that was just created
	/// </summary>
	/// <param name="projectFilePath"></param>
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
		// load Script App Assemblies.
		/*ScriptEngine::LoadAppAssembly(m_Path.parent_path().string() + "\\bin\\ScriptAppLib.dll");
		ScriptEngine::PrintAppAssemblyTypes();*/
	}

	//! sets active scene
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

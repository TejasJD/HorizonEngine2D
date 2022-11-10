#include "pch.h"

#include <cereal/archives/json.hpp>

#include "SceneManager.h"
#include "Scene.h"
#include "Components/Component.h"

namespace Hzn
{
	struct SceneDefaults
	{
		std::string filePath = "default.json";
	};

	static SceneDefaults defaults;

	// filepath of the currently open scene file.
	std::string SceneManager::s_FilePath = std::string();

	std::shared_ptr<Scene> SceneManager::s_Scene = nullptr;


	std::shared_ptr<Scene> SceneManager::load(const std::string& filepath)
	{
		if(s_Scene)
		{
			// serialize and safely close the currently playing scene.
		}

		s_FilePath = filepath;
		// deserialize the coming scene and load it.
		if (!filepath.empty())
		{
			try {
				std::ifstream is(s_FilePath, std::ios::binary);
				cereal::JSONInputArchive inputArchive(is);
				// create the scene and make it valid for actions.
				s_Scene = std::make_shared<Scene>(inputArchive);
				s_Scene->m_Valid = true;
				is.close();
				return s_Scene;
			}
			catch (const cereal::RapidJSONException& e)
			{
				//TODO: Scene corruption dialog.
				const std::string& msg = "scene corruption";
				throw std::runtime_error(msg + e.what());
			}
		}

		// create the scene and make it valid for actions.
		s_Scene = std::make_shared<Scene>();
		s_Scene->m_Valid = true;

		return s_Scene;
	}

	void SceneManager::close(const std::string& filepath)
	{
		// trying to close when there is no scene loaded!
		if(!s_Scene)
		{
			throw std::runtime_error("no scene active!");
		}

		std::ofstream os(filepath, std::ios::binary);

		if(!os)
		{
			throw std::runtime_error("failure on closing scene!");
		}

		cereal::JSONOutputArchive outputArchive(os);
		// serialize the scene before closing.
		s_Scene->serialize(outputArchive);

		os << "\n}\n";

		os.close();

		// invalidate any external pointers to the scene.
		s_Scene->invalidate();
		s_Scene.reset();
	}

	void SceneManager::save(const std::string& filepath)
	{
		// trying to close when there is no scene loaded!
		if (!s_Scene)
		{
			throw std::runtime_error("no scene active!");
		}

		std::ofstream os(filepath, std::ios::binary);

		if (!os)
		{
			throw std::runtime_error("failure on closing scene!");
		}

		cereal::JSONOutputArchive outputArchive(os);
		// serialize the scene before closing.
		s_Scene->serialize(outputArchive);

		os << "\n}\n";

		os.close();

	}
}
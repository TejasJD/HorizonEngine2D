#include "pch.h"

#include <cereal/archives/json.hpp>

#include "Scene.h"
#include "GameObject.h"
#include "HorizonEngine/Components/Component.h"
#include "SceneManager.h"

namespace Hzn
{
	std::shared_ptr<Scene> SceneManager::s_Scene = nullptr;
	std::shared_ptr<Scene> SceneManager::create(const std::filesystem::path& filepath)
	{
		if (s_Scene)
		{
			close();
		}

		// create the scene file at the filepath.
		std::ofstream os(filepath);
		os.close();

		// create the scene and make it valid for actions.
		s_Scene = std::make_shared<Scene>();
		s_Scene->m_Valid = true;
		s_Scene->m_Path = filepath;

		// create a default scene in this case.
		defaultScene();
		return s_Scene;
	}

	std::shared_ptr<Scene> SceneManager::open(const std::filesystem::path& filepath)
	{
		if(s_Scene)
		{
			close();
		}
		if (!filepath.empty()) {
			std::ifstream is(filepath, std::ios::binary);

			HZN_CORE_ASSERT(is, "invalid input file!");

			cereal::JSONInputArchive inputArchive(is);
			// create the scene and make it valid for actions.
			s_Scene = std::make_shared<Scene>(inputArchive);
			s_Scene->m_Valid = true;
			s_Scene->m_Path = filepath;

			is.close();
		}
		return s_Scene;
	}


	void SceneManager::save()
	{
		// save only when active scene.
		if (s_Scene) {
			std::ofstream os(s_Scene->m_Path, std::ios::binary);
			HZN_CORE_ASSERT(os, "output file couldn't be generated");
			cereal::JSONOutputArchive outputArchive(os);
			// serialize the scene before closing.
			s_Scene->serialize(outputArchive);

			os << "\n}\n";
			os.close();
		}
	}

	void SceneManager::close()
	{
		save();
		// invalidate any external pointers to the scene.
		if (s_Scene) {
			s_Scene->invalidate();
		}
		s_Scene.reset();
	}

	void SceneManager::defaultScene()
	{
		Hzn::GameObject mainCamera = s_Scene->createGameObject("MainCamera");

		mainCamera.addComponent<CameraComponent>();
		Hzn::GameObject obj = s_Scene->createGameObject("SampleObject");
		obj.addComponent<RenderComponent>();
	}

}

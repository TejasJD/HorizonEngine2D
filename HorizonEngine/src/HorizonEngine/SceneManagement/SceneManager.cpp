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
		Hzn::GameObject object0 = s_Scene->createGameObject("square 1");
		object0.addComponent<Hzn::TransformComponent>();
		object0.addComponent<Hzn::RenderComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		Hzn::GameObject object1 = s_Scene->createGameObject("square 2");

		object1.addComponent<Hzn::TransformComponent>(glm::vec3(-2.0f, 2.0f, 0.0f), glm::vec3(1.0f));
		object1.addComponent<Hzn::RenderComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		Hzn::GameObject object3 = s_Scene->createGameObject("square 3");

		object3.addComponent<Hzn::TransformComponent>(glm::vec3(-3.0f, -3.0f, 0.0f), glm::vec3(1.0f));
		object3.addComponent<Hzn::RenderComponent>(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

		Hzn::GameObject camera = s_Scene->createGameObject("camera 1");
		camera.addComponent<Hzn::CameraComponent>();
		camera.addComponent<Hzn::TransformComponent>();

		object0.addChild(object1);
		object1.addChild(object3); // object3 local transform will change. 

		object0.duplicate();
	}

}

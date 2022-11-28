#include "pch.h"

#include <cereal/archives/json.hpp>

#include "HorizonEngine/Camera/Camera.h"
#include "Scene.h"
#include "HorizonEngine/Components/Component.h"
#include "SceneManager.h"

namespace Hzn
{
	extern std::string sceneStringStorage;
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

	void SceneManager::update(OrthographicCamera& camera, TimeStep ts)
	{
		if(s_Scene)
		{
			if(s_Scene->m_State == SceneState::Play)
			{
				s_Scene->onUpdate(ts);
			}
			else
			{
				s_Scene->onEditorUpdate(camera, ts);
			}
			
		}
	}

	glm::vec2 SceneManager::resizeViewport(int32_t width, int32_t height)
	{
		glm::vec2 viewportSize = {0, 0};
		if(s_Scene)
		{
			viewportSize = s_Scene->onViewportResize(width, height);
		}
		return viewportSize;
	}

	SceneState SceneManager::getSceneState() { return s_Scene->m_State; }

	void SceneManager::setSceneState(const SceneState& state)
	{
		s_Scene->m_State = state;
	}

	void SceneManager::play()
	{
		if(s_Scene)
		{
			s_Scene->onStart();
		}
	}

	void SceneManager::stop()
	{
		if(s_Scene)
		{
			s_Scene->onStop();
		}
	}


	void SceneManager::save()
	{
		// save only when active scene.
		if (s_Scene) {
			// if scene is playing while we save, we serialize from
			// the snapshot before playing.
			if(s_Scene->m_State == SceneState::Play)
			{
				std::ofstream os(s_Scene->m_Path, std::ios::binary);
				os << sceneStringStorage;
				os.close();
			}
			else
			{
				std::ofstream os(s_Scene->m_Path, std::ios::binary);
				HZN_CORE_ASSERT(os, "output file couldn't be generated");
				cereal::JSONOutputArchive outputArchive(os);
				// serialize the scene before closing.
				s_Scene->serialize(outputArchive);
				os << "\n}\n";
				os.close();
			}
		}
	}

	void SceneManager::close()
	{
		save();
		// invalidate any external pointers to the scene.
		if (s_Scene) {
			//// if state is play, stop the scene.
			if (s_Scene->m_State == SceneState::Play) stop();

			// invalid the scene (so that any hanging references are invalidated.
			s_Scene->invalidate();
		}
		s_Scene.reset();
	}

	void SceneManager::defaultScene()
	{
		/*Hzn::GameObject mainCamera = s_Scene->createGameObject("MainCamera");
		mainCamera.addComponent<CameraComponent>();*/
		/*Hzn::GameObject obj = s_Scene->createGameObject("SampleObject");
		obj.addComponent<RenderComponent>();*/
	}

}

#pragma once

#ifndef HZN_SCENE_H
#define HZN_SCENE_H

#include "HorizonEngine/Core/TimeStep.h"
#include <entt/entt.hpp>

namespace Hzn
{
	class GameObject;
	
	class Scene
	{
		friend class GameObject;
	public:
		Scene();
		~Scene();

		glm::vec2 onViewportResize(uint32_t width, uint32_t height);
		void onUpdate(TimeStep ts);
		GameObject createGameObject();

	private:
		// entt registry for creating game objects.
		entt::registry m_Registry;
		// viewport size of the scene. Helps in maintaining the aspect ratio of the scene.
		glm::vec2 m_lastViewportSize = { 0.0f, 0.0f };
	};

}

#endif // !HZN_SCENE_H

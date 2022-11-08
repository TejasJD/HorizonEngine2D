#include "pch.h"

#include "HorizonEngine/Renderer/Renderer2D.h"

#include "Scene.h"
#include "GameObject.h"
#include "HorizonEngine/Components/Component.h"

namespace Hzn
{
	Scene::Scene(): m_Registry(entt::registry()) {}

	Scene::~Scene()
	{
		m_Registry.clear();
	}

	glm::vec2 Scene::onViewportResize(uint32_t width, uint32_t height)
	{
		// update all the camera components on viewport resize.
		if(glm::vec2(width, height) != m_lastViewportSize)
		{
			const auto& view = m_Registry.view<CameraComponent>();

			for(const auto& entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity).m_Camera;
				camera.setAspectRatio((float)width / height);
			}

			m_lastViewportSize = glm::vec2(width, height);
		}
		return m_lastViewportSize;
	}


	void Scene::onUpdate(TimeStep ts)
	{
		// render objects in the scene through scene update.
		const SceneCamera2D* activeCamera = nullptr;
		const TransformComponent* cameraTransform = nullptr;

		const auto& cameras = m_Registry.view<CameraComponent, TransformComponent>();

		for(const auto& entity : cameras)
		{
			const auto& cameraComponent = cameras.get<CameraComponent>(entity);
			const auto& transformComponent = cameras.get<TransformComponent>(entity);
			if(cameraComponent.m_Primary)
			{
				activeCamera = &cameraComponent.m_Camera;
				cameraTransform = &transformComponent;
			}
		}


		if (activeCamera)
		{
			Renderer2D::beginScene(*activeCamera, *cameraTransform);
			const auto& sprites = m_Registry.view<RenderComponent, TransformComponent>();
			for (const auto& entity : sprites)
			{
				auto [renderComponent, transformComponent] = sprites.get<RenderComponent, TransformComponent>(entity);
				Renderer2D::drawQuad(transformComponent, renderComponent);
			}
			Renderer2D::endScene();
		}
	}

	GameObject Scene::createGameObject()
	{
		return { m_Registry.create(), this };
	}

}

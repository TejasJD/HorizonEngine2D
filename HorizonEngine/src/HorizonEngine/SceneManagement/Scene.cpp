#include "pch.h"

#include "HorizonEngine/Renderer/Renderer2D.h"
#include "Scene.h"
#include "GameObject.h"
#include "HorizonEngine/Components/Component.h"

namespace Hzn
{
	Scene::Scene() : m_Registry(entt::registry()) {}

	Scene::Scene(cereal::JSONInputArchive& inputArchive)
		: m_Registry(entt::registry())
	{
		entt::snapshot_loader loader(m_Registry);
		loader.entities(inputArchive).component<
			NameComponent,
			RelationComponent,
			TransformComponent,
			RenderComponent,
			CameraComponent>(inputArchive);

		// since all valid objects have name components we create a view on name components
		m_Valid = true;
		m_Registry.each([&](auto entity)
			{
				m_Objects.insert({ m_Registry.get<NameComponent>(entity).m_Name, {entity, this} });
			});
		m_Valid = false;
	}

	Scene::~Scene()
	{
		invalidate();
	}

	void Scene::serialize(cereal::JSONOutputArchive& outputArchive)
	{
		entt::snapshot{ m_Registry }
			.entities(outputArchive)
			.component<NameComponent,
		RelationComponent,
		TransformComponent,
		RenderComponent,
		CameraComponent>(outputArchive);
	}

	void Scene::invalidate()
	{
		m_Registry.clear();
		m_Objects.clear();
		m_Valid = false;
	}

	glm::vec2 Scene::onViewportResize(uint32_t width, uint32_t height)
	{
		// update all the camera components on viewport resize.
		if (glm::vec2(width, height) != m_lastViewportSize)
		{
			if (m_Valid)
			{
				const auto& view = m_Registry.view<CameraComponent>();

				for (const auto& entity : view)
				{
					auto& camera = view.get<CameraComponent>(entity).m_Camera;
					camera.setAspectRatio((float)width / height);
				}
			}

			m_lastViewportSize = glm::vec2(width, height);
		}
		return m_lastViewportSize;
	}


	void Scene::onUpdate(TimeStep ts)
	{
		// render objects in the scene through scene update.
		if (m_Valid) {
			const SceneCamera2D* activeCamera = nullptr;
			glm::mat4 cameraTransform = glm::mat4(1.0f);

			const auto& cameras = m_Registry.view<CameraComponent, TransformComponent>();

			for (const auto& entity : cameras)
			{
				const auto& cameraComponent = cameras.get<CameraComponent>(entity);
				const auto& transformComponent = cameras.get<TransformComponent>(entity);
				if (cameraComponent.m_Primary)
				{
					activeCamera = &cameraComponent.m_Camera;
					cameraTransform = transformComponent.getModelMatrix();
				}
			}

			if (activeCamera)
			{
				Renderer2D::beginScene(*activeCamera, cameraTransform);
				const auto& sprites = m_Registry.view<RenderComponent, TransformComponent>();
				for (const auto& entity : sprites)
				{
					auto [renderComponent, transformComponent] = sprites.get<RenderComponent, TransformComponent>(entity);
					Renderer2D::drawQuad(transformComponent.getModelMatrix(), renderComponent);
				}
				Renderer2D::endScene();
			}
		}
	}

	GameObject Scene::createGameObject(const std::string& name)
	{
		if (!m_Valid)
		{
			throw std::runtime_error("Adding game object to invalidated scene!");
		}

		GameObject obj = { m_Registry.create(), this };
		// every valid game object has a name component
		obj.addComponent<NameComponent>(name);
		obj.addComponent<RelationComponent>();
		m_Objects.insert({ name, obj });
		return obj;
	}

	void Scene::destroyGameObject(GameObject& obj)
	{
		if (!m_Valid)
		{
			throw std::runtime_error("trying to get remove game objects from invalidated scene!");
		}
		// break all relations that the game object has in the hierarchy.
		// remove the game object from all objects list.
		m_Registry.destroy(obj.m_ObjectId);
		// remove object from the unordered_map.
		auto list = obj.getChildren();
		
		for(auto& x : list)
		{
			destroyGameObject(x);
		}

		m_Objects.erase(obj.getComponent<NameComponent>());
		obj.m_ObjectId = entt::null;
		obj.m_Scene = nullptr;
	}

	GameObject Scene::getGameObject(const std::string& name)
	{
		if (!m_Valid)
		{
			throw std::runtime_error("trying to get game objects from invalidated scene!");
		}

		auto it = m_Objects.find(name);

		if (it == m_Objects.end())
		{
			throw std::runtime_error("Game object not found!");
		}

		return it->second;
	}

	std::vector<std::string> Scene::allGameObjectNames() const
	{
		std::vector<std::string> names;
		for(const auto& x : m_Objects)
		{
			names.emplace_back(x.first);
		}
		return names;
	}

	std::vector<std::string> Scene::getAllRootObjects() const
	{
		std::vector<std::string> roots;
		for (const auto& x : m_Objects)
		{
			if (x.second.getParent() == GameObject())
			{
				roots.emplace_back(x.first);
			}
		}
		return roots;
	}
}

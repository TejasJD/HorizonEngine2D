#include "pch.h"

#include "HorizonEngine/Renderer/Renderer2D.h"
#include "GameObject.h"
#include "HorizonEngine/Components/Component.h"
#include "HorizonEngine/AssetManagement/AssetManager.h"
#include "Scene.h"


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
				m_GameObjectIdMap.insert({ entt::to_integral(entity), entity });
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
		m_GameObjectIdMap.clear();
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

	void Scene::onEditorUpdate(OrthographicCamera& camera, TimeStep ts) {
		if (m_Valid) {
			Renderer2D::beginScene(camera);
			const auto& sprites = m_Registry.view<RenderComponent, TransformComponent>();
			for (const auto& entity : sprites)
			{
				auto [renderComponent, transformComponent] = sprites.get<RenderComponent, TransformComponent>(entity);
				GameObject obj = getGameObjectById(entt::to_integral(entity));
				auto sprite = 
					AssetManager::getSprite(renderComponent.spritePath, { renderComponent.m_Pos.x, renderComponent.m_Pos.y });
				renderComponent.m_Sprite = sprite;
				Renderer2D::drawSprite(obj.getTransform(), renderComponent, (int32_t)entity);

			}
			Renderer2D::endScene();
		}
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
					GameObject obj = getGameObjectById(entt::to_integral(entity));

					if (!renderComponent.texturePath.empty()) {
						Renderer2D::drawQuad(obj.getTransform(), AssetManager::getTexture(renderComponent.texturePath), renderComponent.m_Color);
					}
					else if (!renderComponent.spritePath.empty())
					{
						Renderer2D::drawSprite(obj.getTransform(),
							AssetManager::getSprite(renderComponent.spritePath, { renderComponent.m_Pos.x, renderComponent.m_Pos.y }),
							renderComponent.m_Color, (int32_t)obj.getObjectId());
					}
					else
					{
						Renderer2D::drawQuad(obj.getTransform(), renderComponent.m_Color);
					}

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
		/*m_Objects.insert({ name, obj });*/
		m_GameObjectIdMap.insert({ entt::to_integral(obj.m_ObjectId), obj.m_ObjectId });
		return obj;
	}

	void Scene::destroyGameObject(GameObject& obj)
	{
		if (!m_Valid)
		{
			throw std::runtime_error("trying to remove game objects from invalidated scene!");
		}

		// delete all the child objects of that object, before we delete that object.
		auto list = obj.getChildren();

		for (auto& x : list)
		{
			destroyGameObject(x);
		}

		// break all relations that the game object has in the hierarchy.
		if (obj.getParent())
		{
			obj.getParent().removeChild(obj);
		}
		// remove the game object from all objects list.
		// remove object from the unordered_map.

		/*m_Objects.erase(obj.getComponent<NameComponent>());*/
		m_GameObjectIdMap.erase(entt::to_integral(obj.m_ObjectId));

		m_Registry.destroy(obj.m_ObjectId);
		obj.m_ObjectId = entt::null;
		obj.m_Scene = nullptr;
	}

	GameObject Scene::getGameObjectById(uint32_t id)
	{
		if (!m_Valid)
		{
			throw std::runtime_error("trying to get game objects from invalidated scene!");
		}

		auto it = m_GameObjectIdMap.find(id);

		if (it == m_GameObjectIdMap.end())
		{
			throw std::runtime_error("Game object not found!");
		}

		return GameObject{ it->second, this };
	}


	std::vector<uint32_t> Scene::getAllRootIds() const
	{
		std::vector<uint32_t> roots;
		if (m_Valid) {
			const auto& alls = m_Registry.view<RelationComponent>();
			for (const auto& entity : alls)
			{
				const auto& parent = m_Registry.get<RelationComponent>(entity).m_Parent;
				if (parent == entt::null) roots.emplace_back(entt::to_integral(entity));
			}
		}
		return roots;
	}

	std::vector<uint32_t> Scene::getAllObjectIds() const
	{
		std::vector<uint32_t> ids;
		for (const auto& x : m_GameObjectIdMap)
		{
			ids.emplace_back(x.first);
		}
		return ids;
	}
}
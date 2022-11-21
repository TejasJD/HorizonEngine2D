#pragma once

#ifndef HZN_COMPONENT_H
#define HZN_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "HorizonEngine/Camera/Camera.h"
#include "HorizonEngine/SceneManagement/GameObject.h"
#include "HorizonEngine/Renderer/Sprite.h"
#include "HorizonEngine/Utils/Math.h"


namespace Hzn
{
	class Scene;

	template<typename>
	inline void display(const GameObject& obj) {}

	struct RelationComponent
	{
		friend class GameObject;
		friend class Scene;
		friend class AssetManager;
		RelationComponent() = default;
		RelationComponent(const RelationComponent& rhs) = default;
		~RelationComponent() = default;

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(cereal::make_nvp("childCount", m_ChildCount),
				cereal::make_nvp("parent", m_Parent),
				cereal::make_nvp("firstChild", m_FirstChild),
				cereal::make_nvp("nextSibling", m_Next),
				cereal::make_nvp("prevSibling", m_Prev));
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(cereal::make_nvp("childCount", (m_ChildCount)),
				cereal::make_nvp("parent", entt::to_integral(m_Parent)),
				cereal::make_nvp("firstChild", entt::to_integral(m_FirstChild)),
				cereal::make_nvp("nextSibling", entt::to_integral(m_Next)),
				cereal::make_nvp("prevSibling", entt::to_integral(m_Prev)));
		}

		bool hasParent() {
			return m_Parent != entt::null;
		}
	private:
		size_t m_ChildCount = 0ULL;
		entt::entity m_Parent{ entt::null };
		entt::entity m_FirstChild{ entt::null };
		entt::entity m_Next{ entt::null };
		entt::entity m_Prev{ entt::null };
	};

	struct NameComponent
	{
		NameComponent() = default;
		NameComponent(const NameComponent& name) = default;
		NameComponent(const std::string& name) : m_Name(name) {}
		~NameComponent() = default;

		operator std::string() const& { return m_Name; }
		operator std::string()& { return m_Name; }

		std::string m_Name;

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_Name);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_Name);
		}
	};

	template<>
	inline void display<NameComponent>(const GameObject& obj)
	{
		char nameString[512];
		memset(nameString, '\0', sizeof(nameString));
		strcpy(nameString, obj.getComponent<NameComponent>().m_Name.c_str());
		if (ImGui::InputText("name", nameString, IM_ARRAYSIZE(nameString), ImGuiInputTextFlags_AutoSelectAll)) {
			obj.getComponent<NameComponent>().m_Name = std::string(nameString);
		}
	}

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& scale)
			: m_Translation(position), m_Scale(scale) {}
		TransformComponent(const glm::vec3& position, float angle, const glm::vec3& scale)
			: m_Translation(position), m_Rotation(angle), m_Scale(scale) {}
		TransformComponent(const TransformComponent& rhs) = default;
		TransformComponent& operator=(const TransformComponent& rhs) = default;
		~TransformComponent() = default;

		glm::vec3 m_Translation = glm::vec3(0.0f);
		glm::vec3 m_Scale = glm::vec3(1.0f);
		float m_Rotation = 0.0f;


		glm::mat4 getModelMatrix() const
		{
			return glm::translate(glm::mat4(1.0f), m_Translation) *
				glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::scale(glm::mat4(1.0f), m_Scale);
		}

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_Rotation);
			ar(m_Translation.x, m_Translation.y, m_Translation.z);
			ar(m_Scale.x, m_Scale.y, m_Scale.z);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_Rotation);
			ar(m_Translation.x, m_Translation.y, m_Translation.z);
			ar(m_Scale.x, m_Scale.y, m_Scale.z);
		}

		void rotateAround(const TransformComponent t, const float angle) {
			float sin = Math::sin(angle * Math::deg2rad);
			float cos = Math::cos(angle * Math::deg2rad);

			float x = m_Translation.x - t.m_Translation.x;
			float y = m_Translation.y - t.m_Translation.y;

			float newX = x * cos - y * sin;
			float newY = x * sin + y * cos;

			m_Translation = glm::vec3(newX + t.m_Translation.x, newY + t.m_Translation.y, m_Translation.z);
		}

		void updateChildren(const GameObject& obj, const glm::vec3 positionDifference, const glm::vec3 scaleFactor, const float rotationDifference
			, const TransformComponent& rootTransform) {
			std::vector<GameObject> children = obj.getChildren();
			TransformComponent t = obj.getComponent<TransformComponent>();
			for (int i = 0; i < children.size(); i++) {
				updateChildren(children.at(i), positionDifference, scaleFactor, rotationDifference, rootTransform);
				auto& transform = children.at(i).getComponent<TransformComponent>();
				transform.m_Translation += positionDifference;
				transform.m_Scale *= scaleFactor;

				// Rotate object around parent by rotationDifference degrees.
				transform.rotateAround(rootTransform, rotationDifference);

				transform.m_Rotation += rotationDifference;
				if (transform.m_Rotation > 180) transform.m_Rotation -= 360;
				if (transform.m_Rotation < -180) transform.m_Rotation += 360;
			}
		}
	};

	template<>
	inline void display<TransformComponent>(const GameObject& obj)
	{
		auto& transform = obj.getComponent<TransformComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Transform", flags)) {
			bool shouldUpdate = false;
			glm::vec3 startTranslation = transform.m_Translation;
			glm::vec3 translationDifference{ 0.0f, 0.0f, 0.0f };

			glm::vec3 startScale = transform.m_Scale;
			glm::vec3 scaleFactor{ 1.0f, 1.0f, 1.0f };

			float startRotation = transform.m_Rotation;
			float rotationDifference = 0.0f;

			if (ImGui::DragFloat3("Position", glm::value_ptr(transform.m_Translation), 0.5f, -50.0f, 50.0f, "%.3f")) {
				shouldUpdate |= true;
				translationDifference = transform.m_Translation - startTranslation;
			}
			if (ImGui::DragFloat3("Scale", glm::value_ptr(transform.m_Scale), 0.5f, -100.0f, 100.0f, "%.3f")) {
				shouldUpdate |= true;
				scaleFactor = transform.m_Scale / startScale;
			}
			if (ImGui::SliderFloat("Rotation", &transform.m_Rotation, -360.0f, 360.0f, "%.3f")) {
				shouldUpdate |= true;

				if (transform.m_Rotation > 180) transform.m_Rotation -= 360;
				if (transform.m_Rotation < -180) transform.m_Rotation += 360;

				rotationDifference = transform.m_Rotation - startRotation;
			}
			ImGui::TreePop();

			if (shouldUpdate) {
				transform.updateChildren(obj, translationDifference, scaleFactor, rotationDifference, transform);
			}
		}
	}

	struct RenderComponent
	{
		RenderComponent() = default;
		RenderComponent(const glm::vec4& color) : m_Color(color) {};
		~RenderComponent() = default;
		// conversion operator for render component.
		operator const glm::vec4() const& { return m_Color; }
		operator glm::vec4()& { return m_Color; }

		glm::vec4 m_Color = glm::vec4(1.0f);

		std::shared_ptr<Hzn::Texture2D> m_Texture;
		std::shared_ptr<Sprite2D> m_Sprite;
		std::string texturePath;
		std::string spritePath;


		std::string m_SpriteHeight;
		std::string m_SpriteWidth;
		std::string m_SpriteY;
		std::string m_SpriteX;

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(spritePath, m_SpriteHeight, m_SpriteWidth, m_SpriteY, m_SpriteX);
			ar(texturePath, m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(spritePath, m_SpriteHeight, m_SpriteWidth, m_SpriteY, m_SpriteX);
			ar(texturePath, m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		}
	};



	template<>
	inline void display<RenderComponent>(const GameObject& obj)
	{
		auto& colorComponent = obj.getComponent<RenderComponent>();
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Render", flags)) {
			ImGui::ColorEdit3("Color", glm::value_ptr(colorComponent.m_Color));

			ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{

					const wchar_t* filepath = (const wchar_t*)payload->Data;
					std::wstring ws(filepath);
					std::string str(ws.begin(), ws.end());
					std::string::size_type nPos1 = str.find_last_of(";");
					std::string::size_type nPos2 = str.find_last_of(";", nPos1 - 1);
					std::string::size_type nPos3 = str.find_last_of(";", nPos2 - 1);
					std::string::size_type nPos4 = str.find_last_of(";", nPos3 - 1);

					if (str.find("-") != std::string::npos)
					{
						std::string spritePath = str.substr(0, str.find("-"));
						std::string spriteHeight = str.substr(nPos1 + 1);
						std::string spriteWidth = str.substr(nPos2 + 1, nPos1 - nPos2 - 1);
						std::string spriteY = str.substr(nPos3 + 1, nPos2 - nPos3 - 1);
						std::string spriteX = str.substr(nPos4 + 1, nPos3 - nPos4 - 1);

						std::shared_ptr<Hzn::Texture2D> texture = Hzn::Texture2D::create(spritePath);
						std::shared_ptr<Hzn::Sprite2D> sprite = Hzn::Sprite2D::create(texture, { std::stoi(spriteX) , std::stoi(spriteY) }, { std::stof(spriteWidth), std::stof(spriteHeight) });
						colorComponent.m_Sprite = sprite;
						colorComponent.texturePath = "";
						colorComponent.spritePath = texture->getPath();

						colorComponent.m_SpriteHeight = spriteHeight;
						colorComponent.m_SpriteWidth = spriteWidth;
						colorComponent.m_SpriteY = spriteY;
						colorComponent.m_SpriteX = spriteX;
					}
					else
					{
						std::shared_ptr<Hzn::Texture2D> texture = Hzn::Texture2D::create(str);
						colorComponent.spritePath = "";
						colorComponent.m_Texture = texture;
						colorComponent.texturePath = texture->getPath();
					}

				}
				ImGui::EndDragDropTarget();
			}

			ImGui::TreePop();
		}
	}

	struct CameraComponent
	{
		CameraComponent(const float aspectRatio = 1.0f, const float zoom = 1.0f)
			: m_Camera(aspectRatio, zoom) {}

		~CameraComponent() = default;
		SceneCamera2D m_Camera;
		bool m_Primary = true;

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_Camera, m_Primary);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_Camera, m_Primary);
		}
	};

	template<>
	inline void display<CameraComponent>(const GameObject& obj)
	{
		auto& cameraComponent = obj.getComponent<CameraComponent>();
		static float m_CameraZoom = cameraComponent.m_Camera.getZoom();
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Camera", flags)) {
			if (ImGui::SliderFloat("Zoom", &m_CameraZoom, 0.25f, 10.0f))
				cameraComponent.m_Camera.setZoom(m_CameraZoom);
			ImGui::TreePop();
		}
	}

	template<typename Component>
	inline void displayIfExists(const GameObject& obj)
	{
		if (obj.hasComponent<Component>())
		{
			display<Component>(obj);
		}
	}

}

#endif

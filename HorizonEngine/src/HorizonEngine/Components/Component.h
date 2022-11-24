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
#include "HorizonEngine/AssetManagement/AssetManager.h"


namespace Hzn
{
	class AssetManager;
	class Scene;

	template<typename>
	inline void display(const GameObject& obj) {}

	struct RelationComponent
	{
		friend class GameObject;
		friend class Scene;
		friend class SceneManager;

		RelationComponent() = default;
		RelationComponent(const RelationComponent& rhs) = default;
		~RelationComponent() = default;

		template<typename Archive>
		void load(Archive& ar)
		{
			uint64_t allUnderSize = 0ULL;
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

		bool hasParent() const {
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
		glm::vec3 m_Rotation = glm::vec3(0.0f);


		glm::mat4 getModelMatrix() const
		{
			glm::mat4 transform = glm::mat4(1.0f);
			transform = glm::translate(transform, m_Translation);
			transform = glm::rotate(transform, glm::radians(m_Rotation.x), { 1, 0, 0 });
			transform = glm::rotate(transform, glm::radians(m_Rotation.y), { 0, 1, 0 });
			transform = glm::rotate(transform, glm::radians(m_Rotation.z), { 0, 0, 1 });
			transform = glm::scale(transform, m_Scale);
			/*glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), { 0, 0, 1 });
			return glm::translate(glm::mat4(1.0f), m_Translation) *
				rotation * glm::scale(glm::mat4(1.0f), m_Scale);*/
			return transform;
		}

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_Translation.x, m_Translation.y, m_Translation.z);
			ar(m_Rotation.x, m_Rotation.y, m_Rotation.z);
			ar(m_Scale.x, m_Scale.y, m_Scale.z);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_Translation.x, m_Translation.y, m_Translation.z);
			ar(m_Rotation.x, m_Rotation.y, m_Rotation.z);
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

		//void translate(const GameObject& obj,  const glm::vec3 positionDifference) {
		//	std::vector<GameObject> children = obj.getChildren();
		//	TransformComponent t = obj.getComponent<TransformComponent>();
		//	for (int i = 0; i < children.size(); i++) {
		//		auto& transform = children.at(i).getComponent<TransformComponent>();
		//		transform.m_Translation += positionDifference;
		//		translate(children.at(i), positionDifference);
		//	}
		//}

		//void scale(const GameObject& obj, const glm::vec3 rootPosition, glm::vec3 newScale) {
		//	std::vector<GameObject> children = obj.getChildren();
		//	TransformComponent t = obj.getComponent<TransformComponent>();
		//	for (int i = 0; i < children.size(); i++) {
		//		HZN_CORE_ERROR(i);
		//		auto& transform = children.at(i).getComponent<TransformComponent>();

		//		std::cout << glm::length((transform.m_Translation - rootPosition) / transform.m_Scale) << std::endl;

		//		// Update position according to scale
		//		transform.m_Translation = rootPosition + ((transform.m_Translation - rootPosition) / transform.m_Scale) * newScale;

		//		transform.m_Scale = newScale;

		//		scale(children.at(i), rootPosition, newScale);
		//	}
		//}

		//void rotate(const GameObject& obj, const float rotationDifference, const TransformComponent& rootTransform) {
		//	std::vector<GameObject> children = obj.getChildren();
		//	TransformComponent t = obj.getComponent<TransformComponent>();
		//	for (int i = 0; i < children.size(); i++) {
		//		auto& transform = children.at(i).getComponent<TransformComponent>();
		//		
		//		// Update position according to rotation
		//		transform.rotateAround(rootTransform, rotationDifference);

		//		// Update rotation
		//		transform.m_Rotation += rotationDifference;
		//		if (transform.m_Rotation > 180) transform.m_Rotation -= 360;
		//		if (transform.m_Rotation < -180) transform.m_Rotation += 360;

		//		rotate(children.at(i), rotationDifference, rootTransform);
		//	}
		//}
	};

	template<>
	inline void display<TransformComponent>(const GameObject& obj)
	{
		auto& transform = obj.getComponent<TransformComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Transform", flags)) {
			ImGui::DragFloat3("Position", glm::value_ptr(transform.m_Translation), 0.25f, -50.0f, 50.0f);
			ImGui::DragFloat3("Scale", glm::value_ptr(transform.m_Scale), 0.25f, 1.0f, 50.0f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(transform.m_Rotation), 1.0f,
				-360.0f, 360.0f);
			ImGui::TreePop();

			/*if (shouldUpdate) {
				transform.updateChildren(obj, transform.m_Translation, translationDifference, scaleFactor, rotationDifference, transform);
			}*/
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

		glm::vec2 m_Pos = glm::vec2(0.0f);


		std::string texturePath;
		std::string spritePath;

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(texturePath, spritePath, m_Pos.x, m_Pos.y);
			ar(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(texturePath, spritePath, m_Pos.x, m_Pos.y);
			ar(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		}
	};



	template<>
	inline void display<RenderComponent>(const GameObject& obj)
	{
		auto& renderComponent = obj.getComponent<RenderComponent>();
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Render", flags)) {
			ImGui::ColorEdit3("Color", glm::value_ptr(renderComponent.m_Color));

			ImGui::Button("Texture", ImVec2(80.0f, 50.0f));
			if (ImGui::BeginDragDropTarget())
			{

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_SPRITE"))
				{

					const wchar_t* filepath = (const wchar_t*)payload->Data;
					std::wstring ws(filepath);
					std::string str(ws.begin(), ws.end());

					std::string::size_type nPos1 = str.find_last_of(";");
					std::string::size_type nPos2 = str.find_last_of(";", nPos1 - 1);

					std::string spriteY = str.substr(nPos1 + 1);
					std::string spriteX = str.substr(nPos2 + 1, nPos1 - nPos2 - 1);

					renderComponent.spritePath = str.substr(0, str.find("-"));
					renderComponent.texturePath = "";
					renderComponent.m_Pos.x = std::stoi(spriteX);
					renderComponent.m_Pos.y = std::stoi(spriteY);

				}
				else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* filepath = (const wchar_t*)payload->Data;
					std::wstring ws(filepath);
					std::string str(ws.begin(), ws.end());

					renderComponent.texturePath = str;
					renderComponent.spritePath = "";

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
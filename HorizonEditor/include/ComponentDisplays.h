#pragma once
#ifndef HZN_COMPONENT_DISPLAYS_H
#define HZN_COMPONENT_DISPLAYS_H
#include <HorizonEngine.h>

namespace Hzn
{
	template<typename... Component>
	struct ComponentGroup {};

	using AllComponents = ComponentGroup<NameComponent, RelationComponent, TransformComponent, RenderComponent, CameraComponent>;

	struct ComponentDisplays
	{
		template<typename T>
		static void display(const GameObject& obj) {}

		template<typename ...Component>
		static void displayIfExists(const GameObject& obj, ComponentGroup<Component...>)
		{
			([&] {
				if (obj.hasComponent<Component>())
				{
					display<Component>(obj);
				}
				}(), ...);
		}
	};

	template<>
	inline void ComponentDisplays::display<NameComponent>(const GameObject& obj)
	{
		char nameString[512];
		memset(nameString, '\0', sizeof(nameString));
		strcpy(nameString, obj.getComponent<NameComponent>().m_Name.c_str());
		if (ImGui::InputText("name", nameString, IM_ARRAYSIZE(nameString), ImGuiInputTextFlags_AutoSelectAll)) {
			obj.getComponent<NameComponent>().m_Name = std::string(nameString);
		}
	}

	template<>
	inline void ComponentDisplays::display<TransformComponent>(const GameObject& obj)
	{
		auto& transform = obj.getComponent<TransformComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Transform", flags)) {
			ImGui::DragFloat3("Position", glm::value_ptr(transform.m_Translation), 0.25f, -50.0f, 50.0f);
			ImGui::DragFloat3("Scale", glm::value_ptr(transform.m_Scale), 0.25f, 1.0f, 50.0f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(transform.m_Rotation), 1.0f,
				-360.0f, 360.0f);
			ImGui::TreePop();
		}
	}

	template<>
	inline void ComponentDisplays::display<RenderComponent>(const GameObject& obj)
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

					renderComponent.spritePath = str.substr(0, str.find_last_of("-"));
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


	template<>
	inline void ComponentDisplays::display<CameraComponent>(const GameObject& obj)
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
}
#endif // !HZN_COMPONENT_DISPLAYS_H

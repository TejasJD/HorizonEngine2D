#pragma once

#ifndef HZN_PHYSICS_COMPONENT_H
#define HZN_PHYSICS_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include "HorizonEngine/SceneManagement/GameObject.h"
#include "HorizonEngine/Physics2D/Physics2DManager.h"

#include "HorizonEngine/Utils/Math.h"

namespace Hzn
{
	// ------------------------------- Rigidbody 2D -------------------------------
	struct Rigidbody2DComponent {
		int m_BodyType = 0; // 0 - Dynamic, 1 - Kinematic, 2 - Static

		float m_Mass = 1.0f;
		float m_LinearDamping = 0.0f;
		float m_AngularDamping = 0.05f;
		float m_GravityScale = 1.0f;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const int bodyType, const float mass, const float linearDamp, const float angularDamp, const float gravityScale) :
			m_BodyType(bodyType),
			m_Mass(mass),
			m_LinearDamping(linearDamp),
			m_AngularDamping(angularDamp),
			m_GravityScale(gravityScale) {}
		~Rigidbody2DComponent() = default;

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_BodyType, m_Mass);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_BodyType, m_Mass);
		}
	};

	template<>
	inline void display<Rigidbody2DComponent>(const GameObject& obj) {
		auto& rigidbodyComponent = obj.getComponent<Rigidbody2DComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Rigidbody 2D", flags)) {
			static const char* items[]{ "Dynamic", "Kinematic", "Static" };
			ImGui::Combo("Type", &rigidbodyComponent.m_BodyType, items, IM_ARRAYSIZE(items));
			ImGui::InputFloat("Mass", &rigidbodyComponent.m_Mass, 0.001f, 0.1f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputFloat("Linear Damping", &rigidbodyComponent.m_LinearDamping, 0.001f, 0.1f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputFloat("Angular Damping", &rigidbodyComponent.m_AngularDamping, 0.001f, 0.1f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputFloat("Gravity Scale", &rigidbodyComponent.m_GravityScale, 0.001f, 0.1f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal);

			ImGui::TreePop();
		}
	}

	// ------------------------------- Box Collider 2D -------------------------------
	struct BoxCollider2DComponent {
		b2PolygonShape m_Shape;

		b2Body* m_Body = nullptr;
		b2Fixture* m_Fixture = nullptr;

		bool m_IsTrigger = false;
		glm::vec2 m_Offset = glm::vec2(0.0f, 0.0f);
		glm::vec2 m_Size = glm::vec2(1.0f, 1.0f);

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const bool isTrigger, const glm::vec2 offset, const glm::vec2 size) : 
			m_IsTrigger(isTrigger), 
			m_Offset(offset), 
			m_Size(size) {}
		~BoxCollider2DComponent() = default;

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_IsTrigger, m_Offset.x, m_Offset.y, m_Size.x, m_Size.y);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_IsTrigger, m_Offset.x, m_Offset.y, m_Size.x, m_Size.y);
		}
	};

	template<>
	inline void display<BoxCollider2DComponent>(const GameObject& obj) {
		auto& boxColliderComponent = obj.getComponent<BoxCollider2DComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Box Collider 2D", flags)) {
			ImGui::Checkbox("Is Trigger", &boxColliderComponent.m_IsTrigger);
			ImGui::InputFloat2("Offset", glm::value_ptr(boxColliderComponent.m_Offset), "%.3f", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputFloat2("Size", glm::value_ptr(boxColliderComponent.m_Size), "%.3f", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal);
			ImGui::TreePop();
		}
	}
}

#endif
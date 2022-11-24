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
		Rigidbody2DComponent() = default;
		~Rigidbody2DComponent() = default;

		int m_BodyType = 0; // 0 - Dynamic, 1 - Kinematic, 2 - Static

		float m_Mass = 1.0f;
		float m_LinearDamping = 0.0f;
		float m_AngularDamping = 0.05f;
		float m_GravityScale = 1.0f;

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

	// ------------------------------- Collider 2D -------------------------------
	struct Collider2DComponent {
		b2PolygonShape m_Shape;

		b2Body* m_Body;
		b2Fixture* m_Fixture;

		bool m_IsTrigger = false;
		glm::vec2 m_Offset = glm::vec2(0.0f, 0.0f);
		glm::vec2 m_Size = glm::vec2(1.0f, 1.0f);

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

		virtual void setShape() = 0;

		virtual void generateBody(GameObject& obj) = 0;
	};

	// ------------------------------- Box Collider 2D -------------------------------
	struct BoxCollider2DComponent : Collider2DComponent {
		BoxCollider2DComponent() {
			setShape();
		}
		~BoxCollider2DComponent() = default;

		void setShape() {
			m_Shape.SetAsBox(m_Size.x, m_Size.y);
		}

		void generateBody(GameObject& obj)
		{
			auto& transformComponent = obj.getComponent<TransformComponent>();

			b2BodyDef bodyDef;
			b2FixtureDef fixtureDef;

			// Calculate body initial position and rotation
			GameObject parent = obj.getParent();
			glm::vec3 startPosition = transformComponent.m_Translation + glm::vec3(m_Offset.x, m_Offset.y, transformComponent.m_Translation.z);
			float rotation = transformComponent.m_Rotation.z;
			while (parent) {
				auto& t = parent.getComponent<TransformComponent>();
				startPosition += t.m_Translation;
				rotation += t.m_Rotation.z;

				parent = parent.getParent();
			}

			bodyDef.position.Set(startPosition.x, startPosition.y);
			bodyDef.angle = rotation;

			// Set body options if a rigidbody is present on the object
			if (obj.hasComponent<Rigidbody2DComponent>()) {
				auto& rigidbody = obj.getComponent<Rigidbody2DComponent>();

				switch (rigidbody.m_BodyType) {
				case 0: // Dynamic body
					bodyDef.type = b2_dynamicBody;
					break;
				case 1: // Kinematic body
					bodyDef.type = b2_kinematicBody;
					break;
				case 2: // Static body
					bodyDef.type = b2_staticBody;
				}

				bodyDef.gravityScale = rigidbody.m_GravityScale;
				bodyDef.linearDamping = rigidbody.m_LinearDamping;
				bodyDef.angularDamping = rigidbody.m_AngularDamping;
			}

			// Create the body and add it to the world
			m_Body = Physics2DManager::addBody(bodyDef);
			m_Body->SetUserData(&obj);

			fixtureDef.isSensor = m_IsTrigger;
			fixtureDef.shape = &m_Shape;

			m_Fixture = m_Body->CreateFixture(&fixtureDef);
		}
	};

	template<>
	inline void display<BoxCollider2DComponent>(const GameObject& obj) {
		auto& boxColliderComponent = obj.getComponent<BoxCollider2DComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Box Collider 2D", flags)) {
			ImGui::Checkbox("Is Trigger", &boxColliderComponent.m_IsTrigger);
			ImGui::InputFloat2("Offset", glm::value_ptr(boxColliderComponent.m_Offset), "%.3f", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal);
			if (ImGui::InputFloat2("Size", glm::value_ptr(boxColliderComponent.m_Size), "%.3f", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal)) {
				boxColliderComponent.setShape();
			}
			ImGui::TreePop();
		}
	}
}

#endif
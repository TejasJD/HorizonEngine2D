#pragma once

#ifndef HZN_PHYSICS_2D_MANAGER_H
#define HZN_PHYSICS_2D_MANAGER_H

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>

#include <HorizonEngine/Core/TimeStep.h>

#include <HorizonEngine/SceneManagement/GameObject.h>
#include <HorizonEngine/Physics2D/Physics2DContactListener.h>

namespace Hzn
{
	class Physics2DManager {
		friend class ProjectManager;
	private:
		static b2Vec2 m_Gravity;
		static bool m_DoSleep;
		static int32 m_VelocityIterations;
		static int32 m_PositionIterations;

		static b2World* m_World;

		static Physics2DContactListener contactListener;

		static void init() {
			m_Gravity = b2Vec2(0, -9.81);
			m_DoSleep = true;
			m_VelocityIterations = 5;
			m_PositionIterations = 3;

			if (!m_World)
			{
				m_World = new b2World(m_Gravity);
				m_World->SetAllowSleeping(m_DoSleep);
				m_World->SetContactListener(&contactListener);
			}
		}

		static void destroy() {
			if (m_World) 
			{
				// Remove and destroy all bodies
				for (b2Body* b = m_World->GetBodyList(); b; b = b->GetNext())
				{
					// Remove and destroy all fixtures of the current body
					for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
					{
						b->DestroyFixture(f);
					}
					m_World->DestroyBody(b);
				}

				// Destroy the world
				delete m_World;
			}
		}

		static void display() {
			ImGui::Text("Physics Settings 2D");
			glm::vec2 gravity(m_Gravity.x, m_Gravity.y);
			if (ImGui::DragFloat2("Gravity", glm::value_ptr(gravity))) {
				m_Gravity = b2Vec2(gravity.x, gravity.y);
			}
			ImGui::Checkbox("Allow sleep", &m_DoSleep);
			ImGui::DragInt("Velocity Iterations", &m_VelocityIterations, 1.0f, 1, 10);
			ImGui::DragInt("Position Iterations", &m_PositionIterations, 1.0f, 1, 10);
		}

	public:
		static void onUpdate(TimeStep ts) {
			m_World->Step(ts, m_VelocityIterations, m_PositionIterations);

			// TODO: Update game objects' transform
		}

		static b2Body* addBody(const b2BodyDef& bodyDef) {
			return m_World->CreateBody(&bodyDef);
		}

		static void removeBody(b2Body& body) {
			// Destroy the body's fixtures
			for (b2Fixture* f = body.GetFixtureList(); f; f = f->GetNext())
			{
				body.DestroyFixture(f);
			}

			// Destroy the body
			m_World->DestroyBody(&body);
		}
	};
}

#endif
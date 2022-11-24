#pragma once

#ifndef HZN_PHYSICS_2D_CONTACT_LISTENER_H
#define HZN_PHYSICS_2D_CONTACT_LISTENER_H

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <box2d/b2_math.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_world_callbacks.h>

namespace Hzn
{
	class Physics2DContactListener : public b2ContactListener
	{
		void BeginContact(b2Contact* contact) {
			b2BodyUserData& bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
			b2BodyUserData& bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

			GameObject* goA;
			GameObject* goB;
			if (bodyUserDataA.pointer)
				goA = reinterpret_cast<GameObject*>(bodyUserDataA.pointer);

			if (bodyUserDataB.pointer)
				goB = reinterpret_cast<GameObject*>(bodyUserDataB.pointer);

			if (goA && goB) {
				if (contact->GetFixtureA()->IsSensor()) goA->onTriggerEnter2D(*goB);
				else goA->onCollisionEnter2D(*goB);
				if (contact->GetFixtureB()->IsSensor()) goB->onTriggerEnter2D(*goA);
				else goB->onCollisionEnter2D(*goA);
			}

		}

		void EndContact(b2Contact* contact) {
			b2BodyUserData& bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
			b2BodyUserData& bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

			GameObject* goA;
			GameObject* goB;
			if (bodyUserDataA.pointer)
				goA = reinterpret_cast<GameObject*>(bodyUserDataA.pointer);

			if (bodyUserDataB.pointer)
				goB = reinterpret_cast<GameObject*>(bodyUserDataB.pointer);

			if (goA && goB) {
				if (contact->GetFixtureA()->IsSensor()) goA->onTriggerExit2D(*goB);
				else goA->onCollisionExit2D(*goB);
				if (contact->GetFixtureB()->IsSensor()) goB->onTriggerExit2D(*goA);
				else goB->onCollisionExit2D(*goA);
			}
		}
	};
}

#endif